#include "include/chessboard/chessboard.h"
#include <climits>
#include <iostream>

void changeColor(std::string &oldColor) {
  if (oldColor == "include/512h/black.png") {
    oldColor = "include/512h/white.png";
  } else {
    oldColor = "include/512h/black.png";
  }
}

void chessboard::initBoard() {
  std::string color = "include/512h/white.png";
  sf::Vector2f coordinates(100.0, 100.0);
  for (auto &line : this->squares) {
    for (auto &square : line) {
      square = new chessboard_square(coordinates, color);
      changeColor(color);
      coordinates += sf::Vector2f(100.0, 0.0);
    }
    coordinates.x = 100.0;
    coordinates.y += 100.0;
    changeColor(color);
  }
}

void chessboard::initVaraible() {
  this->squares.resize(8);
  for (auto &it : this->squares) {
    it.resize(8);
  }
}

void chessboard::initPieces() {
  // black bishop
  [[maybe_unused]] std::vector<sf::Vector2i> empty;
  std::vector<sf::Vector2i> bishopMoves{
      {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},
      {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
      {1, -1},  {2, -2},  {3, -3},  {4, -4},  {5, -5},  {6, -6},  {7, -7},
      {-1, 1},  {-2, 2},  {-3, 3},  {-4, 4},  {-5, 5},  {-6, 6},  {-7, 7},
  };
  (*this->squares[0][2])
      .setRealPiece(
          new pieces(bishopMoves, 3, false, true, "include/512h/bb.png", 4));
  (*this->squares[0][5])
      .setRealPiece(
          new pieces(bishopMoves, 3, false, true, "include/512h/bb.png", 4));

  // white bishop
  (*this->squares[7][2])
      .setRealPiece(
          new pieces(bishopMoves, 3, false, false, "include/512h/wb.png", 4));
  (*this->squares[7][5])
      .setRealPiece(
          new pieces(bishopMoves, 3, false, false, "include/512h/wb.png", 4));

  // blackPawn
  std::vector<sf::Vector2i> blackPawnMoves{{0, 1}, {0, 2}};
  for (auto &it : this->squares[1]) {
    (*it).setRealPiece(
        new pieces(blackPawnMoves, 1, true, true, "include/512h/bp.png", 1));
  }

  // white pawn
  std::vector<sf::Vector2i> whitePawnMoves{{0, -1}, {0, -2}};
  for (auto &it : this->squares[6]) {
    (*it).setRealPiece(
        new pieces(whitePawnMoves, 1, true, false, "include/512h/wp.png", 1));
  }

  // black king
  std::vector<sf::Vector2i> kingMoves{{1, 0}, {-1, 0},  {0, 1},  {0, -1},
                                      {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
  (*this->squares[0][4])
      .setRealPiece(new pieces(kingMoves, INT_MAX, true, true,
                               "include/512h/bkk.png", 6));

  // white king
  (*this->squares[7][4])
      .setRealPiece(new pieces(kingMoves, INT_MAX, true, false,
                               "include/512h/wkk.png", 6));

  // black knight
  std::vector<sf::Vector2i> knightMoves{{1, 2}, {-1, 2}, {1, -2}, {-1, -2},
                                        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
  (*this->squares[0][1])
      .setRealPiece(
          new pieces(knightMoves, 3, false, true, "include/512h/bk.png", 3));
  (*this->squares[0][6])
      .setRealPiece(
          new pieces(knightMoves, 3, false, true, "include/512h/bk.png", 3));

  // white khignt
  (*this->squares[7][1])
      .setRealPiece(
          new pieces(knightMoves, 3, false, false, "include/512h/wk.png", 3));
  (*this->squares[7][6])
      .setRealPiece(
          new pieces(knightMoves, 3, false, false, "include/512h/wk.png", 3));

  // black queen
  std::vector<sf::Vector2i> queenMoves{
      {1, 0},   {2, 0},   {3, 0},   {4, 0},   {5, 0},   {6, 0},   {7, 0},
      {-1, 0},  {-2, 0},  {-3, 0},  {-4, 0},  {-5, 0},  {-6, 0},  {-7, 0},
      {0, 1},   {0, 2},   {0, 3},   {0, 4},   {0, 5},   {0, 6},   {0, 7},
      {0, -1},  {0, -2},  {0, -3},  {0, -4},  {0, -5},  {0, -6},  {0, -7},
      {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},
      {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
      {1, -1},  {2, -2},  {3, -3},  {4, -4},  {5, -5},  {6, -6},  {7, -7},
      {-1, 1},  {-2, 2},  {-3, 3},  {-4, 4},  {-5, 5},  {-6, 6},  {-7, 7},
  };
  (*this->squares[0][3])
      .setRealPiece(
          new pieces(queenMoves, 9, false, true, "include/512h/bq.png", 5));

  // white queen
  (*this->squares[7][3])
      .setRealPiece(
          new pieces(queenMoves, 9, false, false, "include/512h/wq.png", 5));

  // black rook
  std::vector<sf::Vector2i> rookMoves{
      {1, 0},  {2, 0},  {3, 0},  {4, 0},  {5, 0},  {6, 0},  {7, 0},
      {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0},
      {0, 1},  {0, 2},  {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},
      {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}};
  (*this->squares[0][0])
      .setRealPiece(
          new pieces(rookMoves, 4, true, true, "include/512h/br.png", 2));
  (*this->squares[0][7])
      .setRealPiece(
          new pieces(rookMoves, 4, true, true, "include/512h/br.png", 2));

  // white rook
  (*this->squares[7][0])
      .setRealPiece(
          new pieces(rookMoves, 4, true, false, "include/512h/wr.png", 2));
  (*this->squares[7][7])
      .setRealPiece(
          new pieces(rookMoves, 4, true, false, "include/512h/wr.png", 2));
}

chessboard::chessboard() {
  this->initVaraible();
  this->initBoard();
  this->initPieces();
}

chessboard::~chessboard() {
  for (auto &line : this->squares) {
    for (auto &square : line) {
      delete square;
    }
  }
}

void chessboard::renderBoard(sf::RenderWindow &win) {
  for (auto &line : this->squares) {
    for (auto &square : line) {
      (*square).getModel().setPosition((*square).getPosition());
      win.draw((*square).getModel());
    }
  }
  for (auto &line : this->squares) {
    for (auto &square : line) {
      if ((*square).isPiece()) {
        (*square).getPieceOnSquare().getPieceMadel().setPosition(
            (*square).getPosition() + sf::Vector2f{5, 5});
        win.draw((*square).getPieceOnSquare().getPieceMadel());
      }
    }
  }
  for (auto &line : this->squares) {
    for (auto &square : line) {
      if ((*square).getMark() != nullptr) {
        (*(*square).getMark())
            .setPosition((*square).getPosition() + sf::Vector2f{20, 20});
        win.draw((*(*square).getMark()));
      }
    }
  }
}

chessboard_square &chessboard::getSquare(sf::Vector2i coordinates) {
  return *this->squares[coordinates.y][coordinates.x];
}

void chessboard::movePiece(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
  this->getSquare(newSquare).setMovedPiece(getSquare(oldSquare));
}

void chessboard::cleanMark() {
  for (auto &line : (this->squares)) {
    for (auto &square : line) {
      square->squareCleanMark();
    }
  }
}

std::vector<std::vector<chessboard_square *>> &chessboard::getBoard() {
  return this->squares;
}