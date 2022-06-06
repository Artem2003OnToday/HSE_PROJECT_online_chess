#pragma once

#include "include/chessboard/chessboard.h"
#include "include/local_base/local_base.h"
#include "include/main_menu/main_menu.h"
#include "include/piece_upgrade/piece_upgrade.h"
#include "include/start_menu/start_menu.h"
#include "include/timer/timer.h"
#include "server/client.h"
#include <chrono>
#include <iostream>
#include <memory>

class game {
private:
  piece_upgrade *pickPiece = nullptr;
  start_menu *loginMenu = nullptr;
  main_menu *mainMenu = nullptr;
  chessboard *board = nullptr;
  local_base *userdata = nullptr;
  sf::Event window_event;
  sf::Vector2i mouse_window_position;
  void initVariable();
  bool chessboardIsOpen = false;
  bool movingPiece = false;
  sf::Vector2i pickedSquare;
  bool teamTurn = false;
  bool startMenuIsOpened = false;
  bool loginOrPassword = false;
  bool mainMenuIsOpened = false;
  bool kingInTrouble = false;
  int pickingPiece = 0;
  bool swapingKing = false;
  int color;
  /*
    pretty code starts: Artem
  */
  std::unique_ptr<Timer *> timer1player, timer2player;
  std::chrono::steady_clock::time_point time_after_game_starts;
  std::chrono::steady_clock::time_point current_time;
  std::unique_ptr<GameClient *> client;

public:
  game();
  virtual ~game();
  const bool isRunning(sf::RenderWindow &window) const;
  void update_mouse_window_positions(sf::RenderWindow &window);
  sf::Vector2i getInGameCoordinates(sf::RenderWindow &window);
  void update(sf::RenderWindow &window);
  void render(sf::RenderWindow &window);
  void pollEvents(sf::RenderWindow &window);
  bool checkIfPiece(sf::RenderWindow &window);
  [[maybe_unused]] int getPickedType(sf::RenderWindow &window);
  pieces &getPiece(sf::RenderWindow &window);
  void checkMoves(chessboard_square &piece);
  bool boundChecker(sf::Vector2i coord);
  void offlineMode(sf::RenderWindow &window);
  void pawnMoves(chessboard_square &piece);
  void bishopAndRockMoves(chessboard_square &piece);
  void knightMoves(chessboard_square &piece);
  void queenMoves(chessboard_square &piece);
  void kingMoves(chessboard_square &piece);
  void startMenu(sf::RenderWindow &window);
  void mainMenuActions(sf::RenderWindow &window);
  bool checkKingThreat(bool eatOrMove, sf::Vector2i oldPos,
                       sf::Vector2i newPos);
  sf::Vector2i findBlackKing();
  sf::Vector2i findWhiteKing();
  bool anyMoves();

  void update();
  void initGameClient();
};