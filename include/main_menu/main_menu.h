#pragma once
#include <SFML/Graphics.hpp>
// #include <ShellApi.h>
#include "include/start_menu/start_menu.h"
// #include <windows.h>

class main_menu {
private:
  sf::Font *mainMenuFont = nullptr;
  sf::RectangleShape *gameButton = nullptr;
  sf::Text *gameButtonText = nullptr;
  sf::RectangleShape *optionsButton = nullptr;
  sf::Text *optionsButtonText = nullptr;
  sf::RectangleShape *profileButton = nullptr;
  sf::Text *profileButtonText = nullptr;
  sf::RectangleShape *githubButton = nullptr;
  sf::Text *githubButtonText = nullptr;
  void initButtons();

public:
  main_menu();
  ~main_menu();
  void render(sf::RenderWindow &win);
};