#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "nes.h"
#include "rom_file.h"

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(512, 480), "NES");
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
