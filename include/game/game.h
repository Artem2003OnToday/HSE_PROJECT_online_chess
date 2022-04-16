#pragma once
#include "include/chessboard/chessboard.h"

class game {
private:
  chessboard *board;
  sf::RenderWindow *window;
  sf::VideoMode videoMode;
  sf::Event window_event;
  sf::Vector2i mouse_window_position;
  void initVariable();
  [[maybe_unused]] void initWindow();
  void initPieces();
  bool chessboardIsOpen = false;
  bool movingPiece = false;
  sf::Vector2i pickedSquare;
  bool teamTurn = false;

public:
  game();
  virtual ~game();
  const bool isRunning() const;
  void update_mouse_window_positions();
  sf::Vector2i getMousePosition();
  void update();
  void render();
  void pollEvents();
  bool checkIfPiece();
  [[maybe_unused]] int getPickedType();
  pieces &getPiece();
  void checkMoves(pieces &piece);
  bool boundChecker(sf::Vector2i coord);
};
