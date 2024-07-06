#include "../headers/ball.hpp"
#include "../headers/map.hpp"
#include <SFML/Graphics.hpp>

class GolfGame {
public:
    GolfGame(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void handlePress(sf::Event& event);
    void handleRelease(sf::Event& event);
    void handleMouseMoved(sf::Event& event);
    void update(float deltaTime, sf::RenderWindow& window);
    void loadLevel(const std::string& filePath);
    void calculateVelocity();
    void handleLevelUp(sf::RenderWindow& window);
    void displayLevelUpText(sf::RenderWindow& window);

private:
    Ball ball;
    Map map;
    sf::Font font;
    sf::Text strokeText;
    sf::Text levelUpText;
    int strokeCount = 0;
    int curLevel = 1;
    bool validClick = false;
    bool isMousePressed = false;
    sf::Vector2f initialPos;
    sf::Vector2f finalPos;
    sf::Vector2f velDirection;
    float velMagnitude;
};
