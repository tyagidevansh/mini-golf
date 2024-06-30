#ifndef BALL_HPP
#define BALL_HPP

#include "../headers/map.hpp"
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(int x, int y, int radius, const std::string& textureFile);
    void draw(sf::RenderWindow& window);
    void setPos(int x, int y);
    sf::Vector2f getPos();
    void move(float velMagnitude, sf::Vector2f velDirection);
    void update(float deltaTime, const sf::RenderWindow& window, Map& map);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float velMagnitude;
    sf::Vector2f velDirection;
    float friction;
};

#endif // BALL_HPP
