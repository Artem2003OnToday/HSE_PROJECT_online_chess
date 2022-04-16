#pragma once
#include <Graphics.hpp>
#include <string>
#include <vector>

class pieces {
private:
  sf::Image *pieceImage;
  sf::Texture *pieceTexture;
  sf::Sprite *pieceSprite;
  std::vector<sf::Vector2i> movements;
  int cost;
  int type;
  bool specialMove = false;
  bool team;

public:
  pieces(std::vector<sf::Vector2i> moves, int pieceCost, bool special,
         bool pieceTeam, std::string file, int pieceType);
  ~pieces();
  sf::Sprite &getPieceMadel();
  sf::Sprite &setTexture(const std::string &file);
  operator bool();
  void nullSpecial();
  bool getSpecial();
  int getPieceType();
  std::vector<sf::Vector2i> getMovements();
};