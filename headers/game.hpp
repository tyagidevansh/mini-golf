#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "ball.hpp"

class GolfGame {
private:
    Ball ball;
    sf::Vector2f initialPos;
    sf::Vector2f finalPos;
    float velMagnitude;
    sf::Vector2f velDirection;
public:
    GolfGame();
    void draw(sf::RenderWindow& window);
    void handlePress(sf::Event& event);
    void handleRelease(sf::Event& event);
    void calculateVelocity();
    void update(float deltaTime);
};

#endif // GAME_HPP
