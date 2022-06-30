#pragma once
#include "include/chessboard/chessboard.h"

class piece_upgrade {
private:
  std::vector<sf::Image *> pieceImageList;
  std::vector<sf::Texture *> pieceTextureList;
  std::vector<sf::Sprite *> pieceSpriteList;
  sf::RectangleShape *pieceButton;

public:
  piece_upgrade();
  ~piece_upgrade();
  void renderUpgrade(sf::RenderWindow &window);
};