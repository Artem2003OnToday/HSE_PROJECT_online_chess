#include "include/game/game.h"
#include <Graphics.hpp>
#include <iostream>

void run_gui() {
  game runned_game;
  while (runned_game.isRunning()) {
    runned_game.update();
    runned_game.render();
  }
}

int main() {
  run_gui();
  return 0;
}