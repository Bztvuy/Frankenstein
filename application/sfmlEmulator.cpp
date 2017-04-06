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
#include "gamepad.h"
#include "rom_loader.h"
#include "rom_static.h"

using Controller = Frankenstein::Gamepad::ButtonIndex;

std::mutex imageMutex;
sf::Texture screen;

bool isRunning = true;

void emulatorMain(Frankenstein::Nes &nes)
{
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
            screen.update((const sf::Uint8*)nes.ppu.front);
        }
    }
}

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(256, 240), "Frankenstein NES Emulator");
    window.setFramerateLimit(120);
    sf::Sprite tmp;

    tmp.setScale(1.f, 1.f);
    screen.create(256, 240);

    std::string file(argv[1]);
    //Frankenstein::Rom rom(Frankenstein::StaticRom::raw, Frankenstein::StaticRom::length);// Frankenstein::RomLoader::GetRom(file));
    Frankenstein::Rom rom(Frankenstein::RomLoader::GetRom(file));
    Frankenstein::Nes nes(rom);
    std::thread emulatorThr(emulatorMain, std::ref(nes));

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    isRunning = false;
                    window.close();
                    break;
                case sf::Event::KeyPressed: 
                    switch (event.key.code) {
                        case sf::Keyboard::Left:
                            nes.pad1.buttons[Controller::Left] = true;
                            break;
                        case sf::Keyboard::Right:
                            nes.pad1.buttons[Controller::Right] = true;
                            break;
                        case sf::Keyboard::Up:
                            nes.pad1.buttons[Controller::Up] = true;
                            break;
                        case sf::Keyboard::Down:
                            nes.pad1.buttons[Controller::Down] = true;
                            break;
                        case sf::Keyboard::D:
                            nes.pad1.buttons[Controller::B] = true;
                            break;
                        case sf::Keyboard::F:
                            nes.pad1.buttons[Controller::A] = true;
                            break;
                        case sf::Keyboard::S:
                            nes.pad1.buttons[Controller::Select] = true;
                            break;
                        case sf::Keyboard::Return:
                            nes.pad1.buttons[Controller::Start] = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::KeyReleased: 
                    switch (event.key.code) {
                        case sf::Keyboard::Left:
                            nes.pad1.buttons[Controller::Left] = false;
                            break;
                        case sf::Keyboard::Right:
                            nes.pad1.buttons[Controller::Right] = false;
                            break;
                        case sf::Keyboard::Up:
                            nes.pad1.buttons[Controller::Up] = false;
                            break;
                        case sf::Keyboard::Down:
                            nes.pad1.buttons[Controller::Down] = false;
                            break;
                        case sf::Keyboard::D:
                            nes.pad1.buttons[Controller::B] = false;
                            break;
                        case sf::Keyboard::F:
                            nes.pad1.buttons[Controller::A] = false;
                            break;
                        case sf::Keyboard::S:
                            nes.pad1.buttons[Controller::Select] = false;
                            break;
                        case sf::Keyboard::Return:
                            nes.pad1.buttons[Controller::Start] = false;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        {
            std::lock_guard<std::mutex> guard(imageMutex);
            tmp.setTexture(screen, true);
            window.draw(tmp);
        }
        window.display();
    }

    emulatorThr.join();

    return 0;
}
