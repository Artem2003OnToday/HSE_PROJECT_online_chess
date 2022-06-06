#include "game.h"
#include <iostream>

using pii = std::pair<int, int>;

// Private funcs to initialize game window and board
void game::initGameClient() {
  std::cerr << "here" << std::endl;
  client = std::make_unique<GameClient *>(new GameClient(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials())));
  (*client)->current_session_token = (*client)->Auth("login", "password");
  std::cerr << (*client)->current_session_token << std::endl;
}

void game::initVariable() {
  this->loginMenu = new start_menu;
  this->startMenuIsOpened = true;
  timer1player = std::make_unique<Timer *>(
      new Timer("button.png", "Afont.ttf", 1000, sf::Vector2f(700, 0)));
  timer2player = std::make_unique<Timer *>(
      new Timer("button.png", "Afont.ttf", 1000, sf::Vector2f(700, 900)));
  time_after_game_starts = std::chrono::steady_clock::now();
}

// Constructor && destructor
game::game() { this->initVariable(); }

game::~game() {
  if (this->chessboardIsOpen) {
    delete this->board;
  }
  if (this->mainMenuIsOpened) {
    delete this->mainMenu;
  }
  if (this->startMenuIsOpened) {
    delete this->loginMenu;
  }
}
// check if window is still opened
const bool game::isRunning(sf::RenderWindow &window) const {
  return window.isOpen();
}
// mouse position setter
void game::update_mouse_window_positions(sf::RenderWindow &window) {
  current_time = std::chrono::steady_clock::now();
  this->mouse_window_position = sf::Mouse::getPosition(window);
}
// mouse position getter
sf::Vector2i game::getInGameCoordinates(sf::RenderWindow &window) {
  return sf::Vector2i((sf::Mouse::getPosition(window).x - 100) / 100,
                      (sf::Mouse::getPosition(window).y - 100) / 100);
}
// game updater
void game::update(sf::RenderWindow &window) {
  this->update_mouse_window_positions(window);
  this->pollEvents(window);
}
// drawing a board
void game::render(sf::RenderWindow &window) {
  window.clear(sf::Color::Black);
  if (this->chessboardIsOpen) {
    current_time = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(current_time -
                                                         time_after_game_starts)
            .count() >= 1) {
      time_after_game_starts = current_time;
      if (teamTurn) { // fast search
        (*timer1player)->update();
      } else {
        (*timer2player)->update();
      }
    }
    (*timer1player)->draw(window);
    (*timer2player)->draw(window);

    if (!(*timer1player)->getSeconds() || !(*timer2player)->getSeconds()) {
      chessboardIsOpen = false;
      // mainMenuIsOpened = true; !!!!!!!!!!!!!!!
    }

    this->board->renderBoard(window);
    if (this->pickingPiece == 2) {
      this->pickPiece->renderUpgrade(window);
    }
  }
  if (this->startMenuIsOpened) {
    this->loginMenu->renderMenu(window);
  }
  if (this->mainMenuIsOpened) {
    this->mainMenu->render(window);
  }
  current_time = std::chrono::steady_clock::now();
  window.display();
}
// event getter
void game::pollEvents(sf::RenderWindow &window) {
  while (window.pollEvent(this->window_event)) {
    if (this->startMenuIsOpened) {
      this->startMenu(window);
    } else if (this->chessboardIsOpen) {
      this->offlineMode(window);
    } else if (this->mainMenuIsOpened) {
      this->mainMenuActions(window);
    }
  }
}

bool game::checkIfPiece(sf::RenderWindow &window) {
  return (*this->board).getSquare(this->getInGameCoordinates(window)).isPiece();
}

int game::getPickedType(sf::RenderWindow &window) {
  return (*this->board)

      .getSquare(this->getInGameCoordinates(window))
      .getPieceOnSquare()
      .getPieceType();
}

pieces &game::getPiece(sf::RenderWindow &window) {
  return (*this->board)
      .getSquare(this->getInGameCoordinates(window))
      .getPieceOnSquare();
}

void game::checkMoves(chessboard_square &piece) {
  (*this->board)
      .getSquare(piece.getCoordinates())
      .setMark(sf::Color(128, 128, 128, 255));
  if (piece.getPieceOnSquare().getPieceType() == 1) {
    this->pawnMoves(piece);
  } else if (piece.getPieceOnSquare().getPieceType() == 2 ||
             piece.getPieceOnSquare().getPieceType() == 4) {
    this->bishopAndRockMoves(piece);
  } else if (piece.getPieceOnSquare().getPieceType() == 3) {
    this->knightMoves(piece);
  } else if (piece.getPieceOnSquare().getPieceType() == 5) {
    this->queenMoves(piece);
  } else if (piece.getPieceOnSquare().getPieceType() == 6) {
    this->kingMoves(piece);
    // if (piece.getPieceOnSquare().getSpecial()) {
    //    if (!this->board
    //             ->getSquare(piece.getCoordinates() + sf::Vector2i{0, 1})
    //             .isPiece() &&
    //        !this->board
    //             ->getSquare(piece.getCoordinates() + sf::Vector2i{0, 2})
    //             .isPiece() &&
    //        this->board
    //            ->getSquare(piece.getCoordinates() + sf::Vector2i{0, 3})
    //            .isPiece()) {
    //        if (this->board
    //                ->getSquare(piece.getCoordinates() + sf::Vector2i{0, 3})
    //                .getPieceOnSquare()
    //                .getSpecial()) {
    //        }
    //    }
    //    if (!this->board
    //             ->getSquare(piece.getCoordinates() - sf::Vector2i{0, 1})
    //             .isPiece() &&
    //        !this->board
    //             ->getSquare(piece.getCoordinates() - sf::Vector2i{0, 2})
    //             .isPiece() &&
    //        !this->board
    //             ->getSquare(piece.getCoordinates() - sf::Vector2i{0, 3})
    //             .isPiece() &&
    //        this->board
    //            ->getSquare(piece.getCoordinates() - sf::Vector2i{0, 4})
    //            .isPiece()) {
    //        if (this->board
    //                ->getSquare(piece.getCoordinates() - sf::Vector2i{0, 4})
    //                .getPieceOnSquare()
    //                .getSpecial()) {
    //        }
    //    }
    //}
  }
}

bool game::boundChecker(sf::Vector2i coord) {
  if (coord.x < 0 || coord.x > 7)
    return false;
  if (coord.y < 0 || coord.y > 7)
    return false;
  return true;
}

void game::offlineMode(sf::RenderWindow &window) {
  switch (this->window_event.type) {
  case sf::Event::Closed:
    window.close();
    break;
  case sf::Event::MouseButtonPressed:
    std::pair<sf::Vector2i, sf::Vector2i> lastTurn;
    if (this->chessboardIsOpen &&
        this->boundChecker(this->getInGameCoordinates(window))) {
      if (this->window_event.key.code == sf::Mouse::Left) {
        if (this->pickingPiece == 2) {
          if (this->mouse_window_position.y < 100) {
            if (this->mouse_window_position.x > 300 &&
                this->mouse_window_position.x < 400) {
              std::vector<sf::Vector2i> rookMoves{
                  {1, 0},  {2, 0},  {3, 0},  {4, 0},  {5, 0},  {6, 0},
                  {7, 0},  {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0},
                  {-6, 0}, {-7, 0}, {0, 1},  {0, 2},  {0, 3},  {0, 4},
                  {0, 5},  {0, 6},  {0, 7},  {0, -1}, {0, -2}, {0, -3},
                  {0, -4}, {0, -5}, {0, -6}, {0, -7}};
              delete &this->board->getSquare(this->pickedSquare)
                  .getPieceOnSquare();
              std::string file;
              if (!this->teamTurn)
                file = "include/512h/br.png";
              else
                file = "include/512h/wr.png";
              this->board->getSquare(this->pickedSquare)
                  .setRealPiece(
                      new pieces(rookMoves, 4, true, !this->teamTurn, file, 2));
              this->pickingPiece = 0;
              break;
            }
            if (this->mouse_window_position.x > 400 &&
                this->mouse_window_position.x < 500) {
              std::vector<sf::Vector2i> knightMoves{{1, 2},   {-1, 2}, {1, -2},
                                                    {-1, -2}, {2, 1},  {2, -1},
                                                    {-2, 1},  {-2, -1}};
              delete &this->board->getSquare(this->pickedSquare)
                  .getPieceOnSquare();
              std::string file;
              if (!this->teamTurn)
                file = "include/512h/bk.png";
              else
                file = "include/512h/wk.png";
              this->board->getSquare(this->pickedSquare)
                  .setRealPiece(new pieces(knightMoves, 3, false,
                                           !this->teamTurn, file, 3));
              this->pickingPiece = 0;
              break;
            }
            if (this->mouse_window_position.x > 500 &&
                this->mouse_window_position.x < 600) {
              std::vector<sf::Vector2i> bishopMoves{
                  {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},
                  {7, 7},   {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5},
                  {-6, -6}, {-7, -7}, {1, -1},  {2, -2},  {3, -3},  {4, -4},
                  {5, -5},  {6, -6},  {7, -7},  {-1, 1},  {-2, 2},  {-3, 3},
                  {-4, 4},  {-5, 5},  {-6, 6},  {-7, 7},
              };
              delete &this->board->getSquare(this->pickedSquare)
                  .getPieceOnSquare();
              std::string file;
              if (!this->teamTurn)
                file = "include/512h/bb.png";
              else
                file = "include/512h/wb.png";
              this->board->getSquare(this->pickedSquare)
                  .setRealPiece(new pieces(bishopMoves, 3, false,
                                           !this->teamTurn, file, 4));
              this->pickingPiece = 0;
              break;
            }
            if (this->mouse_window_position.x > 600 &&
                this->mouse_window_position.x < 700) {
              std::vector<sf::Vector2i> queenMoves{
                  {1, 0},   {2, 0},   {3, 0},   {4, 0},   {5, 0},   {6, 0},
                  {7, 0},   {-1, 0},  {-2, 0},  {-3, 0},  {-4, 0},  {-5, 0},
                  {-6, 0},  {-7, 0},  {0, 1},   {0, 2},   {0, 3},   {0, 4},
                  {0, 5},   {0, 6},   {0, 7},   {0, -1},  {0, -2},  {0, -3},
                  {0, -4},  {0, -5},  {0, -6},  {0, -7},  {1, 1},   {2, 2},
                  {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},   {-1, -1},
                  {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
                  {1, -1},  {2, -2},  {3, -3},  {4, -4},  {5, -5},  {6, -6},
                  {7, -7},  {-1, 1},  {-2, 2},  {-3, 3},  {-4, 4},  {-5, 5},
                  {-6, 6},  {-7, 7},
              };
              delete &this->board->getSquare(this->pickedSquare)
                  .getPieceOnSquare();
              std::string file;
              if (!this->teamTurn)
                file = "include/512h/bq.png";
              else
                file = "include/512h/wq.png";
              this->board->getSquare(this->pickedSquare)
                  .setRealPiece(new pieces(queenMoves, 9, false,
                                           !this->teamTurn, file, 5));
              this->pickingPiece = 0;
              break;
            }
          }
        } else if (!this->movingPiece) {
          if (this->checkIfPiece(window) &&
              this->teamTurn == this->getPiece(window)) {
            this->movingPiece = true;
            this->pickedSquare = this->getInGameCoordinates(window);
            this->checkMoves(
                this->board->getSquare(this->getInGameCoordinates(window)));
          }
        } else {
          if ((*this->board)
                      .getSquare(this->getInGameCoordinates(window))
                      .getMark() != nullptr &&
              this->getInGameCoordinates(window) != this->pickedSquare) {
            (*this->board)
                .movePiece(this->pickedSquare,
                           this->getInGameCoordinates(window));
            (*this->board)
                .getSquare(this->getInGameCoordinates(window))
                .getPieceOnSquare()
                .nullSpecial();
            this->teamTurn = !(this->teamTurn);
            lastTurn = {pickedSquare, getInGameCoordinates(window)};
            // while (1) {
            (*client)->MakeTurn(lastTurn.first.x, lastTurn.first.y,
                                lastTurn.second.x, lastTurn.second.y);
            std::pair<int, std::pair<pii, pii>> getResponse =
                (*client)->IsAlive();
            if (getResponse.first) {
              board->movePiece(sf::Vector2i(getResponse.second.second.first,
                                            getResponse.second.second.second),
                               sf::Vector2i(getResponse.second.first.first,
                                            getResponse.second.first.second));
            }
            // }

            if (this->chessboardIsOpen)
              (*this->board).cleanMark();
            if (this->pickingPiece == 1) {
              this->pickingPiece = 2;
              this->pickedSquare = this->getInGameCoordinates(window);
            } else if (!this->anyMoves()) {
              this->chessboardIsOpen = false;
              this->mainMenuIsOpened = true;
              this->mainMenu = new main_menu;
              delete this->board;
            }
          }
          if (this->chessboardIsOpen)
            (*this->board).cleanMark();
          this->movingPiece = false;
        }
      }
    }
    break;
  }
  current_time = std::chrono::steady_clock::now();
}

void game::pawnMoves(chessboard_square &piece) {
  // pawn move
  if (this->boundChecker(piece.getCoordinates() +
                         (piece.getPieceOnSquare().getMovements()[0])) &&

      !(*this->board)
           .getSquare(piece.getCoordinates() +
                      (piece.getPieceOnSquare().getMovements()[0]))
           .isPiece() &&

      !this->checkKingThreat(
          false, piece.getCoordinates(),
          piece.getCoordinates() +
              (piece.getPieceOnSquare().getMovements()[0]))) {
    (*this->board)
        .getSquare(piece.getCoordinates() +
                   (piece.getPieceOnSquare().getMovements()[0]))
        .setMark(sf::Color::Blue);

    if ((piece.getCoordinates() + (piece.getPieceOnSquare().getMovements()[0]))
                .y %
            7 ==
        0) {
      this->pickingPiece = 1;
    }

    if (piece.getPieceOnSquare().getSpecial() &&

        this->boundChecker(piece.getCoordinates() +
                           (piece.getPieceOnSquare().getMovements()[1])) &&

        !(*this->board)
             .getSquare(piece.getCoordinates() +
                        (piece.getPieceOnSquare().getMovements()[1]))
             .isPiece() &&

        !this->checkKingThreat(
            false, piece.getCoordinates(),
            piece.getCoordinates() +
                (piece.getPieceOnSquare().getMovements()[1]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     (piece.getPieceOnSquare().getMovements()[1]))
          .setMark(sf::Color::Blue);
    }
  }

  // pawn attack
  if (piece.getPieceOnSquare()) {
    if (this->boundChecker(piece.getCoordinates() + sf::Vector2i(1, 1)) &&
        (*this->board)
            .getSquare(piece.getCoordinates() + sf::Vector2i(1, 1))
            .isPiece() &&
        (*this->board)
                .getSquare(piece.getCoordinates() + sf::Vector2i(1, 1))
                .getPieceOnSquare() != piece.getPieceOnSquare() &&
        !this->checkKingThreat(true, piece.getCoordinates(),
                               piece.getCoordinates() + sf::Vector2i(1, 1))) {
      (*this->board)
          .getSquare(piece.getCoordinates() + sf::Vector2i(1, 1))
          .setMark(sf::Color::Red);
    }
    if (this->boundChecker(piece.getCoordinates() + sf::Vector2i(-1, 1)) &&
        (*this->board)
            .getSquare(piece.getCoordinates() + sf::Vector2i(-1, 1))
            .isPiece() &&
        (*this->board)
                .getSquare(piece.getCoordinates() + sf::Vector2i(-1, 1))
                .getPieceOnSquare() != piece.getPieceOnSquare() &&
        !this->checkKingThreat(true, piece.getCoordinates(),
                               piece.getCoordinates() + sf::Vector2i(-1, 1))) {
      (*this->board)
          .getSquare(piece.getCoordinates() + sf::Vector2i(-1, 1))
          .setMark(sf::Color::Red);
    }
  } else {
    if (this->boundChecker(piece.getCoordinates() + sf::Vector2i(1, -1)) &&
        (*this->board)
            .getSquare(piece.getCoordinates() + sf::Vector2i(1, -1))
            .isPiece() &&
        (*this->board)
                .getSquare(piece.getCoordinates() + sf::Vector2i(1, -1))
                .getPieceOnSquare() != piece.getPieceOnSquare() &&
        !this->checkKingThreat(true, piece.getCoordinates(),
                               piece.getCoordinates() + sf::Vector2i(1, -1))) {
      (*this->board)
          .getSquare(piece.getCoordinates() + sf::Vector2i(1, -1))
          .setMark(sf::Color::Red);
    }
    if (this->boundChecker(piece.getCoordinates() + sf::Vector2i(-1, -1)) &&
        (*this->board)
            .getSquare(piece.getCoordinates() + sf::Vector2i(-1, -1))
            .isPiece() &&
        (*this->board)
                .getSquare(piece.getCoordinates() + sf::Vector2i(-1, -1))
                .getPieceOnSquare() != piece.getPieceOnSquare() &&
        !this->checkKingThreat(true, piece.getCoordinates(),
                               piece.getCoordinates() + sf::Vector2i(-1, -1))) {
      (*this->board)
          .getSquare(piece.getCoordinates() + sf::Vector2i(-1, -1))
          .setMark(sf::Color::Red);
    }
  }
}

void game::bishopAndRockMoves(chessboard_square &piece) {
  for (uint32_t i = 0; i < 7; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                   .getSquare(piece.getCoordinates() +
                              piece.getPieceOnSquare().getMovements()[i])
                   .isPiece() &&
               (*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 7; i < 14; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                   .getSquare(piece.getCoordinates() +
                              piece.getPieceOnSquare().getMovements()[i])
                   .isPiece() &&
               (*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 14; i < 21; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                   .getSquare(piece.getCoordinates() +
                              piece.getPieceOnSquare().getMovements()[i])
                   .isPiece() &&
               (*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 21; i < 28; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                   .getSquare(piece.getCoordinates() +
                              piece.getPieceOnSquare().getMovements()[i])
                   .isPiece() &&
               (*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
}

void game::knightMoves(chessboard_square &piece) {
  for (auto &it : piece.getPieceOnSquare().getMovements()) {
    if (!this->boundChecker(piece.getCoordinates() + it))
      continue;

    if (!(*this->board).getSquare(piece.getCoordinates() + it).isPiece()) {
      if (!this->checkKingThreat(false, piece.getCoordinates(),
                                 piece.getCoordinates() + it))
        (*this->board)
            .getSquare(piece.getCoordinates() + it)
            .setMark(sf::Color::Blue);
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() + it)
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(true, piece.getCoordinates(),
                                      piece.getCoordinates() + it)) {
      (*this->board)
          .getSquare(piece.getCoordinates() + it)
          .setMark(sf::Color::Red);
    }
  }
}

void game::queenMoves(chessboard_square &piece) {
  for (uint32_t i = 0; i < 7; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 7; i < 14; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 14; i < 21; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 21; i < 28; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 28; i < 35; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 35; i < 42; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 42; i < 49; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
  for (uint32_t i = 49; i < 56; i++) {
    if (!this->boundChecker(piece.getCoordinates() +
                            piece.getPieceOnSquare().getMovements()[i]))
      break;

    if (!(*this->board)
             .getSquare(piece.getCoordinates() +
                        piece.getPieceOnSquare().getMovements()[i])
             .isPiece()) {
      if (!this->checkKingThreat(
              false, piece.getCoordinates(),
              piece.getCoordinates() +
                  (piece.getPieceOnSquare().getMovements()[i]))) {
        (*this->board)
            .getSquare(piece.getCoordinates() +
                       piece.getPieceOnSquare().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else
        continue;
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() +
                                  piece.getPieceOnSquare().getMovements()[i])
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(
                   true, piece.getCoordinates(),
                   piece.getCoordinates() +
                       (piece.getPieceOnSquare().getMovements()[i]))) {
      (*this->board)
          .getSquare(piece.getCoordinates() +
                     piece.getPieceOnSquare().getMovements()[i])
          .setMark(sf::Color::Red);
      break;
    } else {
      break;
    }
  }
}

void game::kingMoves(chessboard_square &piece) {
  for (auto &it : piece.getPieceOnSquare().getMovements()) {
    if (!this->boundChecker(piece.getCoordinates() + it))
      continue;

    if (!(*this->board).getSquare(piece.getCoordinates() + it).isPiece()) {
      if (!this->checkKingThreat(false, piece.getCoordinates(),
                                 piece.getCoordinates() + it))
        (*this->board)
            .getSquare(piece.getCoordinates() + it)
            .setMark(sf::Color::Blue);
    } else if ((*this->board)
                       .getSquare(piece.getCoordinates() + it)
                       .getPieceOnSquare() != piece.getPieceOnSquare() &&
               !this->checkKingThreat(true, piece.getCoordinates(),
                                      piece.getCoordinates() + it)) {
      (*this->board)
          .getSquare(piece.getCoordinates() + it)
          .setMark(sf::Color::Red);
    }
  }
}
void game::startMenu(sf::RenderWindow &window) {
  switch (window_event.type) {
  case sf::Event::Closed:
    window.close();
    break;
  case sf::Event::MouseButtonPressed:
    if (this->mouse_window_position.x > 50 &&
        this->mouse_window_position.x < 300) {
      if (this->mouse_window_position.y > 50 &&
          this->mouse_window_position.y < 85) {
        this->loginOrPassword = false;
      } else if (this->mouse_window_position.y > 115 &&
                 this->mouse_window_position.y < 150) {
        this->loginOrPassword = true;
      }
    }
    if (this->mouse_window_position.x > 25 &&
        this->mouse_window_position.x < 110 &&
        this->mouse_window_position.y > 150 &&
        this->mouse_window_position.x < 175) {
      this->startMenuIsOpened = false;
      this->mainMenuIsOpened = true;
      delete this->loginMenu;
      window.create(sf::VideoMode(1000, 1000), "Chess project",
                    sf::Style::Close);
      this->mainMenu = new main_menu;
    }
    break;
  case sf::Event::TextEntered:
    if (!this->loginOrPassword) {
      if (this->window_event.text.unicode == 8) {
        if (this->loginMenu->getLoginString().getSize()) {
          this->loginMenu->getLoginString().erase(
              this->loginMenu->getLoginString().getSize() - 1, 1);
          this->loginMenu->getLoginText().setString(
              this->loginMenu->getLoginString());
        }
      } else if (this->window_event.text.unicode < 128) {
        this->loginMenu->getLoginString() += this->window_event.text.unicode;
        this->loginMenu->getLoginText().setString(
            this->loginMenu->getLoginString());
      }
    } else {
      if (this->window_event.text.unicode == 8) {
        if (this->loginMenu->getPasswordString().getSize()) {
          this->loginMenu->getPasswordString().erase(
              this->loginMenu->getPasswordString().getSize() - 1, 1);
          this->loginMenu->getHidedString().erase(
              this->loginMenu->getHidedString().getSize() - 1, 1);
          this->loginMenu->getPasswordText().setString(
              this->loginMenu->getHidedString());
        }
      } else if (this->window_event.text.unicode < 128) {
        this->loginMenu->getPasswordString() += this->window_event.text.unicode;
        this->loginMenu->getHidedString() += '*';
        this->loginMenu->getPasswordText().setString(
            this->loginMenu->getHidedString());
      }
    }
    break;
  }
}

sf::Vector2i game::findBlackKing() {
  for (int line = 0; line < 8; line++) {
    for (int square = 0; square < 8; square++)
      if (this->board->getSquare({line, square}).isPiece())
        if (this->board->getSquare({line, square})
                    .getPieceOnSquare()
                    .getPieceType() == 6 &&
            this->board->getSquare({line, square}).getPieceOnSquare()) {
          return {line, square};
        }
  }
  return {-1, -1};
}

sf::Vector2i game::findWhiteKing() {
  for (int line = 0; line < 8; line++) {
    for (int square = 0; square < 8; square++)
      if (this->board->getSquare({line, square}).isPiece())
        if (this->board->getSquare({line, square})
                    .getPieceOnSquare()
                    .getPieceType() == 6 &&
            !this->board->getSquare({line, square}).getPieceOnSquare()) {
          return {line, square};
        }
  }
  return {-1, -1};
}

bool game::anyMoves() {
  for (auto &line : (this->board->getBoard())) {
    for (auto &square : line) {
      if (square->isPiece()) {
        if (this->teamTurn == square->getPieceOnSquare()) {
          this->checkMoves(*square);
        }
      }
    }
  }
  for (auto &line : (this->board->getBoard())) {
    for (auto &square : line) {
      if (square->getMark() != nullptr) {
        if (square->getMark()->getFillColor() == sf::Color::Blue ||
            square->getMark()->getFillColor() == sf::Color::Red) {
          this->board->cleanMark();
          return true;
        }
      }
    }
  }
  this->board->cleanMark();
  return false;
}

bool game::checkKingThreat(bool eatOrMove, sf::Vector2i oldPos,
                           sf::Vector2i newPos) {
  if (eatOrMove) {
    pieces *eatenPiece = &this->board->getSquare(newPos).getPieceOnSquare();
    this->board->getSquare(newPos).setRealPiece(nullptr);
    this->board->movePiece(oldPos, newPos);
    sf::Vector2i myKingCoordinates;
    if (this->teamTurn) {
      myKingCoordinates = this->findBlackKing();
    } else {
      myKingCoordinates = this->findWhiteKing();
    }

    std::vector<sf::Vector2i> bishopQueenRookMoves{
        {1, 0},   {2, 0},   {3, 0},   {4, 0},   {5, 0},   {6, 0},   {7, 0},
        {-1, 0},  {-2, 0},  {-3, 0},  {-4, 0},  {-5, 0},  {-6, 0},  {-7, 0},
        {0, 1},   {0, 2},   {0, 3},   {0, 4},   {0, 5},   {0, 6},   {0, 7},
        {0, -1},  {0, -2},  {0, -3},  {0, -4},  {0, -5},  {0, -6},  {0, -7},
        {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},
        {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
        {1, -1},  {2, -2},  {3, -3},  {4, -4},  {5, -5},  {6, -6},  {7, -7},
        {-1, 1},  {-2, 2},  {-3, 3},  {-4, 4},  {-5, 5},  {-6, 6},  {-7, 7},
    };
    std::vector<sf::Vector2i> knightMoves{{1, 2}, {-1, 2}, {1, -2}, {-1, -2},
                                          {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    std::vector<sf::Vector2i> kingMoves{{1, 0}, {-1, 0},  {0, 1},  {0, -1},
                                        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

    if (this->teamTurn) {
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{1, 1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{1, 1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{1, 1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            !(*this->board)
                 .getSquare(myKingCoordinates + sf::Vector2i{1, 1})
                 .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        }
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{-1, 1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{-1, 1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{-1, 1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            !(*this->board)
                 .getSquare(myKingCoordinates + sf::Vector2i{-1, 1})
                 .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        }
    } else {
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{1, -1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{1, -1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{1, -1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            (*this->board)
                .getSquare(myKingCoordinates + sf::Vector2i{1, -1})
                .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        }
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{-1, -1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{-1, -1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{-1, -1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            (*this->board)
                .getSquare(myKingCoordinates + sf::Vector2i{-1, -1})
                .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        }
    }

    for (uint32_t i = 0; i < 7; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 7; i < 14; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 14; i < 21; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 21; i < 28; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 28; i < 35; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 35; i < 42; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 42; i < 49; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 49; i < 56; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          this->board->getSquare(newPos).setRealPiece(eatenPiece);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (auto &movement : knightMoves) {
      if (!this->boundChecker(myKingCoordinates + movement))
        continue;
      if (&(*this->board)
               .getSquare(myKingCoordinates + movement)
               .getPieceOnSquare() == nullptr)
        continue;
      if ((*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare() != this->teamTurn &&
          (*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare()
                  .getPieceType() == 3) {
        this->board->movePiece(newPos, oldPos);
        this->board->getSquare(newPos).setRealPiece(eatenPiece);
        return true;
      }
    }
    for (auto &movement : kingMoves) {
      if (!this->boundChecker(myKingCoordinates + movement))
        continue;
      if (&(*this->board)
               .getSquare(myKingCoordinates + movement)
               .getPieceOnSquare() == nullptr)
        continue;
      if ((*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare() != this->teamTurn &&
          (*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare()
                  .getPieceType() == 6) {
        this->board->movePiece(newPos, oldPos);
        this->board->getSquare(newPos).setRealPiece(eatenPiece);
        return true;
      }
    }
    this->board->movePiece(newPos, oldPos);
    this->board->getSquare(newPos).setRealPiece(eatenPiece);
    return false;
  } else {
    this->board->movePiece(oldPos, newPos);
    sf::Vector2i myKingCoordinates;
    if (this->teamTurn) {
      myKingCoordinates = this->findBlackKing();
    } else {
      myKingCoordinates = this->findWhiteKing();
    }

    std::vector<sf::Vector2i> bishopQueenRookMoves{
        {1, 0},   {2, 0},   {3, 0},   {4, 0},   {5, 0},   {6, 0},   {7, 0},
        {-1, 0},  {-2, 0},  {-3, 0},  {-4, 0},  {-5, 0},  {-6, 0},  {-7, 0},
        {0, 1},   {0, 2},   {0, 3},   {0, 4},   {0, 5},   {0, 6},   {0, 7},
        {0, -1},  {0, -2},  {0, -3},  {0, -4},  {0, -5},  {0, -6},  {0, -7},
        {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},
        {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7},
        {1, -1},  {2, -2},  {3, -3},  {4, -4},  {5, -5},  {6, -6},  {7, -7},
        {-1, 1},  {-2, 2},  {-3, 3},  {-4, 4},  {-5, 5},  {-6, 6},  {-7, 7},
    };
    std::vector<sf::Vector2i> knightMoves{{1, 2}, {-1, 2}, {1, -2}, {-1, -2},
                                          {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    std::vector<sf::Vector2i> kingMoves{{1, 0}, {-1, 0},  {0, 1},  {0, -1},
                                        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

    if (this->teamTurn) {
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{1, 1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{1, 1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{1, 1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            !(*this->board)
                 .getSquare(myKingCoordinates + sf::Vector2i{1, 1})
                 .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          return true;
        }
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{-1, 1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{-1, 1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{-1, 1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            !(*this->board)
                 .getSquare(myKingCoordinates + sf::Vector2i{-1, 1})
                 .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          return true;
        }
    } else {
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{1, -1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{1, -1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{1, -1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            (*this->board)
                .getSquare(myKingCoordinates + sf::Vector2i{1, -1})
                .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          return true;
        }
      if (this->boundChecker(myKingCoordinates + sf::Vector2i{-1, -1}) &&
          &(*this->board)
                  .getSquare(myKingCoordinates + sf::Vector2i{-1, -1})
                  .getPieceOnSquare() != nullptr)
        if ((*this->board)
                    .getSquare(myKingCoordinates + sf::Vector2i{-1, -1})
                    .getPieceOnSquare()
                    .getPieceType() == 1 &&
            (*this->board)
                .getSquare(myKingCoordinates + sf::Vector2i{-1, -1})
                .getPieceOnSquare()) {
          this->board->movePiece(newPos, oldPos);
          return true;
        }
    }

    for (uint32_t i = 0; i < 7; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 7; i < 14; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 14; i < 21; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 21; i < 28; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 2) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 28; i < 35; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 35; i < 42; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 42; i < 49; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (uint32_t i = 49; i < 56; i++) {
      if (!this->boundChecker(myKingCoordinates + bishopQueenRookMoves[i]))
        break;
      if (&(*this->board)
               .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
               .getPieceOnSquare() == nullptr) {
        continue;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() != this->teamTurn) {
        if ((*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 5 ||
            (*this->board)
                    .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                    .getPieceOnSquare()
                    .getPieceType() == 4) {
          this->board->movePiece(newPos, oldPos);
          return true;
        } else
          break;
      } else if ((*this->board)
                     .getSquare(myKingCoordinates + bishopQueenRookMoves[i])
                     .getPieceOnSquare() == this->teamTurn) {
        break;
      }
    }
    for (auto &movement : knightMoves) {
      if (!this->boundChecker(myKingCoordinates + movement))
        continue;
      if (&(*this->board)
               .getSquare(myKingCoordinates + movement)
               .getPieceOnSquare() == nullptr)
        continue;
      if ((*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare() != this->teamTurn &&
          (*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare()
                  .getPieceType() == 3) {
        this->board->movePiece(newPos, oldPos);
        return true;
      }
    }
    for (auto &movement : kingMoves) {
      if (!this->boundChecker(myKingCoordinates + movement))
        continue;
      if (&(*this->board)
               .getSquare(myKingCoordinates + movement)
               .getPieceOnSquare() == nullptr)
        continue;
      if ((*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare() != this->teamTurn &&
          (*this->board)
                  .getSquare(myKingCoordinates + movement)
                  .getPieceOnSquare()
                  .getPieceType() == 6) {
        this->board->movePiece(newPos, oldPos);
        return true;
      }
    }
    this->board->movePiece(newPos, oldPos);
    return false;
  }
}

void game::mainMenuActions(sf::RenderWindow &window) {
  switch (window_event.type) {
  case sf::Event::Closed:
    window.close();
    break;
  case sf::Event::MouseButtonPressed:
    if (this->mouse_window_position.x > 250 &&
        this->mouse_window_position.x < 750) {
      if (this->mouse_window_position.y > 200 &&
          this->mouse_window_position.y < 300) {
        delete this->mainMenu;
        this->mainMenuIsOpened = false;
        this->chessboardIsOpen = true;
        initGameClient();
        (*client)->StartGame();
        pii have = (*client)->SearchGame();
        while (!have.first) {
          std::this_thread::sleep_for(std::chrono::milliseconds(5));
          have = (*client)->SearchGame();
        }
        color = have.second;
        std::cerr << (color ? "BLACK" : "WHITE") << std::endl;
        teamTurn = false;
        this->board = new chessboard;
        this->pickPiece = new piece_upgrade;
      } else if (this->mouse_window_position.y > 310 &&
                 this->mouse_window_position.y < 410) {
        // settings
      } else if (this->mouse_window_position.y > 420 &&
                 this->mouse_window_position.y < 520) {
        // profile
      } else if (this->mouse_window_position.y > 530 &&
                 this->mouse_window_position.y < 630) {
        // github
        // ShellExecute(0, 0,
        //              L"https://github.com/Artem2003OnToday/"
        //              L"HSE_PROJECT_online_chess",
        //              NULL, NULL, SW_SHOW);
      }
    }
    break;
  }
}