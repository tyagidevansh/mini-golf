#include "../headers/ball.hpp"
#include "../headers/map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
    void showTitleScreen(sf::RenderWindow& window, float elapsedTime);
    void showEndScreen(sf::RenderWindow& window);

private:
    void updateTextPositions(const sf::RenderWindow& window);
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void playScoreSound();

    Ball ball;
    Map map;
    sf::Font font;
    sf::Text strokeText;
    sf::Text holeText;
    sf::Text levelUpText;
    int strokeCount = 0;
    int totalStrokes = 0;
    int curLevel = 1;
    bool validClick = false;
    bool isMousePressed = false;
    sf::Vector2f initialPos;
    sf::Vector2f finalPos;
    sf::Vector2f velDirection;
    float velMagnitude;
    sf::Texture titleTexture;
    sf::Sprite titleSprite;
    sf::Text titleText;
    sf::Text bottomText;
    sf::Texture splashBgTexture;
    sf::Sprite splashBg;
    sf::Sprite splashBg2;
    sf::Texture starTexture;
    sf::Sprite star1;
    sf::Sprite star2;
    sf::Sprite star3;
    sf::Texture boardTexture;
    sf::Sprite board;
    sf::Music bgMusic;            
    sf::SoundBuffer scoreSoundBuffer;
    sf::Sound scoreSound;   
};
