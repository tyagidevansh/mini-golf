#ifndef BALL_HPP
#define BALL_HPP

#include "map.hpp"
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(int x, int y, int radius, const std::string& textureFile);
    void draw(sf::RenderWindow& window);
    void setPos(int x, int y);
    sf::Vector2f getPos();
    void move(float velMagnitude, sf::Vector2f velDirection);
    void update(float deltaTime, const sf::RenderWindow& window, Map& map);
    bool getHoleStatus();
    void setHoleStatus();
    void reset();

private:
    sf::Vector2f initialPos;
    sf::Sprite sprite;
    sf::Texture texture;
    float velMagnitude;
    sf::Vector2f velDirection;
    float friction;
    bool scaling;
    bool isHoleComplete = false;
};

#endif // BALL_HPP
