#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <bitset>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <mutex>
#include <thread>

#include "cpu.h"
#include "nes.h"
#include "rom_file.h"

std::mutex imageMutex;
sf::Image img;
bool isRunning = true;

void emulatorMain(std::string filename)
{
    Frankenstein::FileRom rom(filename);
    Frankenstein::Nes nes(rom);

    std::ofstream out("debug2.txt", std::ios::out | std::ios::binary);
    out << "EX.TIME|PPU|PC  |SVABDIZC|A |X |Y |Instruction| Hex data" << std::endl;

    while (isRunning) {
        std::stringstream instString;
        auto op = nes.cpu.OpCode();
        auto instr = nes.cpu.instructions[op];
        instString << std::setfill('0') << std::setw(3) << std::hex
                   << (unsigned int)nes.ppu.Cycle << "|";
        instString << std::setfill('0') << std::setw(4) << std::hex
                   << (unsigned int)nes.cpu.registers.PC << "|";
        instString << std::bitset<8>(nes.cpu.registers.P) << "|";
        instString << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int)nes.cpu.registers.A << "|";
        instString << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int)nes.cpu.registers.X << "|";
        instString << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int)nes.cpu.registers.Y << "|";
        instString << std::setfill(' ') << std::setw(11)
                   << instr.name << "| ";

        int max = instr.size == 0 ? 3 : instr.size;
        for (int i = 0; i < max; i++) {
            instString << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)nes.cpu.Operand(i);
            instString << " ";
        }

        auto begin = std::chrono::steady_clock::now();
        nes.Step();
        auto end = std::chrono::steady_clock::now();

        out << std::setfill(' ') << std::setw(5) << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "ns|";
        out << instString.str();
        out << std::endl;

        if (nes.ppu.vblankOccured) {
            std::lock_guard<std::mutex> guard(imageMutex);
            img.create(1024, 960, sf::Color::Black);
            for (unsigned int i = 0; i < 256; ++i) {
                for (unsigned int j = 0; j < 240; ++j) {
                    sf::Color color(nes.ppu.front[i + 256 * j].red, nes.ppu.front[i + 256 * j].green, nes.ppu.front[i + 256 * j].blue);
                    img.setPixel(i * 4 + 0, j * 4 + 0, color);
                    img.setPixel(i * 4 + 1, j * 4 + 0, color);
                    img.setPixel(i * 4 + 2, j * 4 + 0, color);
                    img.setPixel(i * 4 + 3, j * 4 + 0, color);
                    img.setPixel(i * 4 + 0, j * 4 + 1, color);
                    img.setPixel(i * 4 + 1, j * 4 + 1, color);
                    img.setPixel(i * 4 + 2, j * 4 + 1, color);
                    img.setPixel(i * 4 + 3, j * 4 + 1, color);
                    img.setPixel(i * 4 + 0, j * 4 + 2, color);
                    img.setPixel(i * 4 + 1, j * 4 + 2, color);
                    img.setPixel(i * 4 + 2, j * 4 + 2, color);
                    img.setPixel(i * 4 + 3, j * 4 + 2, color);
                    img.setPixel(i * 4 + 0, j * 4 + 3, color);
                    img.setPixel(i * 4 + 1, j * 4 + 3, color);
                    img.setPixel(i * 4 + 2, j * 4 + 3, color);
                    img.setPixel(i * 4 + 3, j * 4 + 3, color);
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(1024, 960), "Frankenstein NES Emulator");
    window.setFramerateLimit(30);
    sf::Texture screen;
    sf::Sprite tmp;
    sf::Text text;
    screen.create(1024, 960);
    img.create(1024, 960, sf::Color::Black);

    std::string file(argv[1]);
    std::thread emulatorThr(emulatorMain, file);

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                isRunning = false;
                window.close();
            }

            {
                std::lock_guard<std::mutex> guard(imageMutex);
                screen.loadFromImage(img);
            }
            tmp.setTexture(screen, false);
            window.draw(tmp);
            window.display();
        }
    }

    emulatorThr.join();

    return 0;
}
