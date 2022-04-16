#include "game.h"
#include <iostream>

// Private funcs to initialize game window and board
void game::initVariable() {
  this->window = nullptr;
  this->board = nullptr;
}

void game::initWindow() {
  this->chessboardIsOpen = false;
  this->videoMode.height = 1000;
  this->videoMode.width = 1000;
  this->window =
      new sf::RenderWindow(this->videoMode, "Chess project", sf::Style::Close);
  this->board = new chessboard;
}
void game::initPieces() {}
// Constructor && destructor
game::game() {
  this->initVariable();
  this->initWindow();
}

game::~game() {
  delete this->board;
  delete this->window;
}
// check if window is still opened
const bool game::isRunning() const { return this->window->isOpen(); }
// mouse position setter
void game::update_mouse_window_positions() {
  this->mouse_window_position = sf::Mouse::getPosition(*this->window);
}
// mouse position getter
sf::Vector2i game::getMousePosition() {

  return sf::Vector2i((sf::Mouse::getPosition(*this->window).x - 100) / 100,
                      (sf::Mouse::getPosition(*this->window).y - 100) / 100);
}
// game updater
void game::update() {
  this->update_mouse_window_positions();
  this->pollEvents();
  // std::cout << this->checkIfPiece();
}
// drawing a board
void game::render() {
  this->window->clear(sf::Color::Black);
  if (this->chessboardIsOpen) {
    this->board->renderBoard(*this->window);
  }
  this->window->display();
}
// event getter
void game::pollEvents() {
  while (this->window->pollEvent(this->window_event)) {
    switch (this->window_event.type) {
    case sf::Event::Closed:
      this->window->close();
      break;
    case sf::Event::KeyPressed:
      if (this->window_event.key.code == sf::Keyboard::Escape)
        this->window->close();
      if (this->window_event.key.code == sf::Keyboard::Enter)
        this->chessboardIsOpen = !(this->chessboardIsOpen);
      break;
    case sf::Event::MouseButtonPressed:
      if (this->chessboardIsOpen &&
          this->boundChecker(this->getMousePosition())) {
        if (this->window_event.key.code == sf::Mouse::Left) {
          if (!this->movingPiece) {
            if (this->checkIfPiece() && this->teamTurn == this->getPiece()) {
              this->movingPiece = true;
              this->pickedSquare = this->getMousePosition();
              this->checkMoves(this->getPiece());
            }
          } else {
            if ((*this->board).getSquare(this->getMousePosition()).getMark() !=
                nullptr) {
              (*this->board)
                  .movePiece(this->pickedSquare, this->getMousePosition());
              this->teamTurn = !(this->teamTurn);
              this->movingPiece = false;
              (*this->board).cleanMark();
            }
          }
        }
        break;
      }
    }
  }
}

bool game::checkIfPiece() {
  return (*this->board).getSquare(this->getMousePosition()).isPiece();
}

int game::getPickedType() {
  return (*this->board)
      .getSquare(this->getMousePosition())
      .getPieceOnSquare()
      .getPieceType();
}

pieces &game::getPiece() {
  return (*this->board).getSquare(this->getMousePosition()).getPieceOnSquare();
}

void game::checkMoves(pieces &piece) {
  if (piece.getPieceType() == 1) {
    // pawn move
    if (this->boundChecker(this->getMousePosition() +
                           (this->getPiece().getMovements()[0])) &&
        !(*this->board)
             .getSquare(this->getMousePosition() +
                        (this->getPiece().getMovements()[0]))
             .isPiece()) {
      (*this->board)
          .getSquare(this->getMousePosition() +
                     (this->getPiece().getMovements()[0]))
          .setMark(sf::Color::Blue);
      if (piece.getSpecial() &&
          this->boundChecker(this->getMousePosition() +
                             (this->getPiece().getMovements()[1])) &&
          !(*this->board)
               .getSquare(this->getMousePosition() +
                          (this->getPiece().getMovements()[1]))
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       (this->getPiece().getMovements()[1]))
            .setMark(sf::Color::Blue);
      }
    }
    // pawn attack
    if (piece) {
      if (this->boundChecker(this->getMousePosition() + sf::Vector2i(1, 1)) &&
          (*this->board)
              .getSquare(this->getMousePosition() + sf::Vector2i(1, 1))
              .isPiece() &&
          (*this->board)
                  .getSquare(this->getMousePosition() + sf::Vector2i(1, 1))
                  .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() + sf::Vector2i(1, 1))
            .setMark(sf::Color::Red);
      }
      if (this->boundChecker(this->getMousePosition() + sf::Vector2i(-1, 1)) &&
          (*this->board)
              .getSquare(this->getMousePosition() + sf::Vector2i(-1, 1))
              .isPiece() &&
          (*this->board)
                  .getSquare(this->getMousePosition() + sf::Vector2i(-1, 1))
                  .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() + sf::Vector2i(-1, 1))
            .setMark(sf::Color::Red);
      }
    } else {
      if (this->boundChecker(this->getMousePosition() + sf::Vector2i(1, -1)) &&
          (*this->board)
              .getSquare(this->getMousePosition() + sf::Vector2i(1, -1))
              .isPiece() &&
          (*this->board)
                  .getSquare(this->getMousePosition() + sf::Vector2i(1, -1))
                  .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() + sf::Vector2i(1, -1))
            .setMark(sf::Color::Red);
      }
      if (this->boundChecker(this->getMousePosition() + sf::Vector2i(-1, -1)) &&
          (*this->board)
              .getSquare(this->getMousePosition() + sf::Vector2i(-1, -1))
              .isPiece() &&
          (*this->board)
                  .getSquare(this->getMousePosition() + sf::Vector2i(-1, -1))
                  .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() + sf::Vector2i(-1, -1))
            .setMark(sf::Color::Red);
      }
    }
  } else if (piece.getPieceType() == 2 || piece.getPieceType() == 4) {
    for (uint32_t i = 0; i < 7; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 7; i < 14; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 14; i < 21; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 21; i < 28; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
  } else if (piece.getPieceType() == 3) {
    for (auto &it : piece.getMovements()) {
      if (!this->boundChecker(this->getMousePosition() + it))
        continue;

      if (!(*this->board).getSquare(this->getMousePosition() + it).isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() + it)
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() + it)
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() + it)
            .setMark(sf::Color::Red);
      }
    }
  } else if (piece.getPieceType() == 5) {
    for (uint32_t i = 0; i < 7; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 7; i < 14; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 14; i < 21; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 21; i < 28; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 28; i < 35; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 35; i < 42; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 42; i < 49; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
    for (uint32_t i = 49; i < 56; i++) {
      if (!this->boundChecker(this->getMousePosition() +
                              this->getPiece().getMovements()[i]))
        break;

      if (!(*this->board)
               .getSquare(this->getMousePosition() +
                          this->getPiece().getMovements()[i])
               .isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() +
                                this->getPiece().getMovements()[i])
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() +
                       this->getPiece().getMovements()[i])
            .setMark(sf::Color::Red);
        break;
      } else {
        break;
      }
    }
  } else if (piece.getPieceType() == 6) {
    for (auto &it : piece.getMovements()) {
      if (!this->boundChecker(this->getMousePosition() + it))
        continue;

      if (!(*this->board).getSquare(this->getMousePosition() + it).isPiece()) {
        (*this->board)
            .getSquare(this->getMousePosition() + it)
            .setMark(sf::Color::Blue);
      } else if ((*this->board)
                     .getSquare(this->getMousePosition() + it)
                     .getPieceOnSquare() != piece) {
        (*this->board)
            .getSquare(this->getMousePosition() + it)
            .setMark(sf::Color::Red);
      }
    }
  }
}

bool game::boundChecker(sf::Vector2i coord) {
  if (coord.x < 0 || coord.x > 7)
    return false;
  if (coord.y < 0 || coord.y > 7)
    return false;
  return true;
}
