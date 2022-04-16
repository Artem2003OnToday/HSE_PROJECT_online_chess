#include "chessboard_square.h"

chessboard_square::~chessboard_square() { delete this->figureOnSquare; }

chessboard_square::chessboard_square(sf::Vector2f coordinates,
                                     std::string file) {
  this->square_coordinate = coordinates;
  this->squareImage = new sf::Image;
  this->squareSprite = new sf::Sprite;
  this->squareTexture = new sf::Texture;
  (*this->squareImage).loadFromFile(file);
  (*this->squareTexture).loadFromImage(*this->squareImage);
  (*this->squareSprite).setTexture(*this->squareTexture);
  (*this->squareSprite).setScale(sf::Vector2f(0.2, 0.2));
}

sf::Vector2f &chessboard_square::getPosition() {
  return this->square_coordinate;
}

sf::Sprite &chessboard_square::getModel() { return *this->squareSprite; }

void chessboard_square::setRealPiece(pieces *piece) {
  this->figureOnSquare = piece;
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
  this->figureOnSquare->nullSpecial();
  otherSquare.pieceIsHere = false;
  otherSquare.figureOnSquare = nullptr;
}

sf::CircleShape *chessboard_square::getMark() { return this->mark; }

void chessboard_square::setMark(sf::Color color) {
  this->mark = new sf::CircleShape(30);
  (*this->mark).setFillColor(color);
}

void chessboard_square::cleanMark() {
  if (this->mark == nullptr)
    return;
  delete this->mark;
  this->mark = nullptr;
}
