#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <fstream>
#include <iomanip>

#include "nes.h"
#include "rom_file.h"

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(512, 480), "Frankenstein NES Emulator");
    window.setFramerateLimit(30);
    sf::Texture screen;
    sf::Image img;
    sf::Sprite tmp;
    sf::Text text;
    screen.create(256, 240);
    img.create(256, 240);

    std::string file(argv[1]);
    Frankenstein::FileRom rom(file);
    Frankenstein::Nes nes(rom);
    
    std::ofstream out("debug.txt", std::ios::out|std::ios::binary);
    // Debug info to file

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

        {
            auto op = nes.cpu.OpCode();
            auto instr = nes.cpu.instructions[op];
            out << std::setfill('0') << std::setw(4) << std::hex << (unsigned int)nes.cpu.registers.PC<< ": ";
            out << std::setfill(' ') << std::setw(9) << instr.name << ": ";
            int max = instr.size == 0 ? 3 : instr.size;
            for(int i = 0; i < max; i++) {
                out << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)nes.cpu.Operand(i);
                out << " ";
            }
            out << std::endl;
        }

        nes.Step();
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
