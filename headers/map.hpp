#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

enum CellType {
  EMPTY,
  OBSTACLE,
};

class Map {
private:
  std::vector<std::vector<int>> grid;
  float cellSize;
  sf::RenderWindow& window;
  sf::Texture obstacleTexture;
  sf::Sprite obstacle;

public:
  Map(int width, int height, sf::RenderWindow& window, const std::string& textureFile);
  void loadMap(const std::vector<std::vector<int>>& mapData);
  void loadMapFromFile(const std::string& filePath);
  void draw();
  bool isObstacle(int x, int y);
  sf::Vector2i getCellCoords(sf::Vector2f position);
};

#endif // !MAP_HPP