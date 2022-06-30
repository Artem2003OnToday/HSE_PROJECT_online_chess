#include "chessboard_square.h"

chessboard_square::~chessboard_square() {
  delete this->figureOnSquare;
  delete this->squareImage;
  delete this->squareSprite;
  delete this->squareTexture;
}

chessboard_square::chessboard_square(sf::Vector2f coordinates,
                                     std::string file) {
  this->square_coordinate = coordinates;
  this->squareImage = new sf::Image; // NOLINT
  this->squareSprite = new sf::Sprite;
  this->squareTexture = new sf::Texture;
  (*this->squareImage).loadFromFile(file);
  (*this->squareTexture).loadFromImage(*this->squareImage);
  (*this->squareSprite).setTexture(*this->squareTexture);
  (*this->squareSprite).setScale(sf::Vector2f{0.2f, 0.2f});
}

sf::Vector2f &chessboard_square::getPosition() {
  return this->square_coordinate;
}

sf::Vector2i chessboard_square::getCoordinates() {
  return {(int)this->square_coordinate.x / 100 - 1,
          (int)this->square_coordinate.y / 100 - 1};
}

sf::Sprite &chessboard_square::getModel() { return *this->squareSprite; }

void chessboard_square::setRealPiece(pieces *piece) {
  this->figureOnSquare = piece;
  if (piece != nullptr)
    this->pieceIsHere = true;
}

pieces &chessboard_square::getPieceOnSquare() { return *figureOnSquare; }

bool chessboard_square::isPiece() { return pieceIsHere; }

void chessboard_square::setMovedPiece(chessboard_square &otherSquare) {
  this->pieceIsHere = true;
  if (this->figureOnSquare != nullptr) {
    delete this->figureOnSquare;
  }
  this->figureOnSquare = (otherSquare.figureOnSquare);
  otherSquare.pieceIsHere = false;
  otherSquare.figureOnSquare = nullptr;
}

sf::CircleShape *chessboard_square::getMark() { return this->mark; }

void chessboard_square::setMark(sf::Color color) {
  if (this->mark != nullptr)
    return;
  this->mark = new sf::CircleShape(30);
  (*this->mark).setFillColor(color);
}

void chessboard_square::squareCleanMark() {
  if (this->mark == nullptr)
    return;
  else {
    delete this->mark;
    this->mark = nullptr;
  }
}