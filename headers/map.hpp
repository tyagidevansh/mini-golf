#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

enum CellType {
  EMPTY,
  OBSTACLE,
  SAND,
  HOLE,
  WATER,
};

class Map {
private:
  std::vector<std::vector<int>> grid;
  float cellSize;
  sf::RenderWindow& window;
  sf::Texture obstacleTexture;
  sf::Sprite obstacle;
  sf::Texture holeTexture;
  sf::Sprite hole;
  sf::Texture flagTexture;
  sf::Sprite flag;
  sf::Texture sandTexture;
  sf::Sprite sand;
  std::vector<sf::Texture> waterTextures;
  std::vector<sf::Sprite> waterSprites;
  int currentWaterFrame;
  float animationTimer;
  float animationSpeed;

  sf::Texture waterTexture;  // Base water texture
  sf::Sprite waterSprite;
  sf::Shader waterShader;    // Shader for water effect

public:
  Map(int width, int height, sf::RenderWindow& window, const std::string& textureFile, const std::string& holeTextureFile);
  void loadMap(const std::vector<std::vector<int>>& mapData);
  void loadMapFromFile(const std::string& filePath);
  void draw();
  bool isObstacle(int x, int y);
  bool isSand(int x, int y);
  bool isWater(int x, int y);
  bool isHole(int x, int y);
  sf::Vector2f getHoleCenter();
  sf::Vector2i getCellCoords(sf::Vector2f position);
};

#endif // !MAP_HPP