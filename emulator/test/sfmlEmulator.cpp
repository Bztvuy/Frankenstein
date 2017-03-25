#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "nes.h"
#include "rom_file.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 960), "NES");
    window.setFramerateLimit(30);
    sf::Texture screen;
    sf::Image img;
    sf::Sprite tmp;
    sf::Text text;
    screen.create(1024, 960);
    img.create(1024, 960);
    Frankenstein::FileRom rom("");
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
                    img.setPixel(i * 4, j * 4, color);
                    img.setPixel(i * 4 + 1, j * 4, color);
                    img.setPixel(i * 4 + 2, j * 4, color);
                    img.setPixel(i * 4 + 3, j * 4, color);
                    img.setPixel(i * 4, j * 4 + 1, color);
                    img.setPixel(i * 4 + 1, j * 4 + 1, color);
                    img.setPixel(i * 4 + 2, j * 4 + 1, color);
                    img.setPixel(i * 4 + 3, j * 4 + 1, color);
                    img.setPixel(i * 4, j * 4 + 2, color);
                    img.setPixel(i * 4 + 1, j * 4 + 2, color);
                    img.setPixel(i * 4 + 2, j * 4 + 2, color);
                    img.setPixel(i * 4 + 3, j * 4 + 2, color);
                    img.setPixel(i * 4, j * 4 + 3, color);
                    img.setPixel(i * 4 + 1, j * 4 + 3, color);
                    img.setPixel(i * 4 + 2, j * 4 + 3, color);
                    img.setPixel(i * 4 + 3, j * 4 + 3, color);
                }
            }
        }

        screen.loadFromImage(img);
        tmp.setTexture(screen, true);
        window.draw(tmp);
        window.display();
    }
    
    return 0;
}
