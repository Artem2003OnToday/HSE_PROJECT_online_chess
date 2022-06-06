#pragma once
#include <Graphics.hpp>
#include <iostream>

class start_menu {
private:
  sf::RectangleShape *acceptButton = nullptr;
  sf::Text *textButton = nullptr;

  sf::RectangleShape *passwordSign = nullptr;
  sf::Text *passwordSignText = nullptr;
  sf::RectangleShape *passwordLine = nullptr;
  sf::String *inputPassword = nullptr;
  sf::Text *passwordString = nullptr;

  sf::RectangleShape *loginSign = nullptr;
  sf::Text *loginSignText = nullptr;
  sf::RectangleShape *loginLine = nullptr;
  sf::Text *loginString = nullptr;
  sf::String *inputLogin = nullptr;

  sf::Font *menuFont = nullptr;
  sf::String *hidedPasswordString;
  void initMenu();
  void initButton();
  void initPassword();
  void initLogin();

public:
  sf::String &getLoginString();
  sf::Text &getLoginText();
  sf::String &getPasswordString();
  sf::Text &getPasswordText();
  sf::String &getHidedString();
  sf::Font &getFont();
  start_menu();
  ~start_menu();
  void renderMenu(sf::RenderWindow &win);
};