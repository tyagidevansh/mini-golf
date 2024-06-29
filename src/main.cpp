#include <SFML/Graphics.hpp>
#include "../headers/game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mini Golf");
    GolfGame game;
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                game.handlePress(event);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                game.handleRelease(event);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        game.update(deltaTime);
        game.draw(window);
        window.display();
    }

    return 0;
}
