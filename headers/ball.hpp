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
    bool getHoleStatus();
    void setHoleStatus();
    void reset();
    void updatePowerIndicator(float power, sf::Vector2f direction);

private:
    sf::Vector2f initialPos;
    sf::Sprite sprite;
    sf::Texture texture;
    float velMagnitude;
    sf::Vector2f velDirection;
    float friction;
    float scaleFactor;
    bool scaling;
    bool isHoleComplete = false;

    sf::RectangleShape powerIndicator; // Add power indicator
};

#endif // BALL_HPP
