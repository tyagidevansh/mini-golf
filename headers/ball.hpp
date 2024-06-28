#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>

class Ball {
private:
    sf::CircleShape shape;
    float velMagnitude;
    sf::Vector2f velDirection;
    const float friction;
public:
    Ball(int x, int y, int radius);
    void draw(sf::RenderWindow& window);
    void setPos(int x, int y);
    sf::Vector2f getPos();
    void move(float velMagnitude, sf::Vector2f velDirection);
    void update(float deltaTime);
};

#endif // BALL_HPP
