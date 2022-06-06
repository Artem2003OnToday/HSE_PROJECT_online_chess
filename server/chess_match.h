#ifndef CHESS_MATCH_H_
#define CHESS_MATCH_H_

#include <iostream>
#include <vector>

namespace chess_match {
enum BOARD_COLOR { WHITE, BLACK };
const int HEIGHT = 8;
const int WIDTH = 8;

class Turn {
private:
  int first_row, first_column;
  int secon_row, secon_column;

public:
  Turn() = default;
  Turn(int first_row_, int first_column_, int secon_row_, int secon_column_)
      : first_row(first_row_), first_column(first_column_),
        secon_row(secon_row_), secon_column(secon_column_) {}
  std::pair<int, int> get_start_position() const {
    return std::pair<int, int>(first_row, first_column);
  }
  std::pair<int, int> get_finish_position() const {
    return std::pair<int, int>(secon_row, secon_column);
  }
  std::string get_in_border(int y, int x) const {
    return std::to_string(x + 'a') + std::to_string(y + '0');
  }
  std::string make_readable() const {
    return get_in_border(first_row, first_column) + "-" +
           get_in_border(secon_row, secon_column);
  }
};
class MatchBoard {
private:
  std::vector<std::vector<int>> brd;

public:
  MatchBoard()
      : brd(std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH))) {}
  MatchBoard(const std::vector<std::vector<int>> &brd_) : brd(brd_) {}
  void add_motion(const Turn &turn) {
    using first = y;
    using second = x;

    std::pair<int, int> fm = turn.get_start_position();
    std::pair<int, int> sm = turn.get_finish_position();
    brd[sm.y][sm.x] = std::exchange(brd[fm.y][fm.x], 0);
  }
};
class MatchByTurn {
private:
  std::vector<std::string> match;

public:
  MatchByTurn() = default;
  void add_motion(const Turn &turn) {
    using first = y;
    using second = x;
    match.push_back(turn.make_readable());
  }
};
} // namespace chess_match

#endif // CHESS_MATCH_H_