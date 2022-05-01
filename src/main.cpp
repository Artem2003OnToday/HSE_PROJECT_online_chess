#include "include/game/game.h"

void test_SFML() {
  game runned_game;
  while (runned_game.isRunning()) {
    runned_game.update();
    runned_game.render();
  }
}

void test_gRPC() {
  
}

int main() {
  test_SFML();
  test_gRPC();
  return 0;
}