#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <bitset>

#include "nes.h"
#include "cpu.h"
#include "rom_file.h"

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(512, 480), "Frankenstein NES Emulator");
    sf::Texture screen;
    sf::Image img;
    sf::Sprite tmp;
    sf::Text text;
    screen.create(256, 240);
    img.create(256, 240);

    std::string file(argv[1]);
    Frankenstein::FileRom rom(file);
    Frankenstein::Nes nes(rom);
    
    std::ofstream out("debug.txt", std::ios::out | std::ios::binary);
    out << "EX.TIME|PC  |SVABDIZC|A |X |Y |Instruction| Hex data" << std::endl;

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        std::stringstream instString;
        auto op = nes.cpu.OpCode();
        auto instr = nes.cpu.instructions[op];
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
        for(int i = 0; i < max; i++) {
            instString << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)nes.cpu.Operand(i);
            instString << " ";
        }

        auto begin = std::chrono::high_resolution_clock::now();
        nes.Step();
        auto end = std::chrono::high_resolution_clock::now();

        out << std::setfill(' ') << std::setw(5) << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << "ns|";
        out << instString.str();
        out << std::endl;

        if (nes.cpu.nmiOccurred){
            for (unsigned int i = 0; i < 256; ++i) {
                for (unsigned int j = 0; j < 240; ++j) {
                    sf::Color color(nes.ppu.front[i][j].red, nes.ppu.front[i][j].green, nes.ppu.front[i][j].blue);
                    img.setPixel(i, j, color);
                }
            }
            screen.loadFromImage(img);
            tmp.setTexture(screen, true);
            window.draw(tmp);
        }

        window.display();
    }

    return 0;
}
