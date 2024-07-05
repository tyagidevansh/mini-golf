#include <SFML/Graphics.hpp>
#include "../headers/game.hpp"

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    unsigned int width = desktopMode.width;
    unsigned int height = desktopMode.height;
    if (width * 9 > height * 16) {
        width = height * 16 / 9;
    } else {
        height = width * 9 / 16;
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "Mini Golf", sf::Style::Fullscreen);
    GolfGame game(window);
    sf::Clock clock;

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/background.jpg")) {
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        float(width) / backgroundTexture.getSize().x,
        float(height) / backgroundTexture.getSize().y
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    game.handlePress(event);
                    break;
                case sf::Event::MouseButtonReleased:
                    game.handleRelease(event);
                    break;
                case sf::Event::MouseMoved:
                    game.handleMouseMoved(event);
                    break;
                default:
                    break;
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear();
        window.draw(backgroundSprite);
        game.update(deltaTime, window);
        game.draw(window);
        window.display();
    }

    return 0;
}
