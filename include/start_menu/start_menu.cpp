#include "start_menu.h"

void start_menu::initMenu() {
  this->menuFont = new sf::Font;
  (*this->menuFont).loadFromFile("include/512h/font.ttf");
  this->initButton();
  this->initLogin();
  this->initPassword();
}
void start_menu::initButton() {
  this->acceptButton = new sf::RectangleShape({87, 26});
  (*this->acceptButton).setPosition({25, 149});

  this->textButton = new sf::Text;
  (*this->textButton).setFont(*this->menuFont);
  (*this->textButton).setString("Sign in :)");
  (*this->textButton).setPosition({25, 142});
  (*this->textButton).setCharacterSize(31);
  (*this->textButton).setFillColor(sf::Color::Black);
}

void start_menu::initPassword() {
  this->passwordSign = new sf::RectangleShape({102, 25});
  (*this->passwordSign).setPosition({25, 87});

  this->passwordSignText = new sf::Text;
  (*this->passwordSignText).setString("PASSWORD");
  (*this->passwordSignText).setFont(*this->menuFont);
  (*this->passwordSignText).setPosition({25, 80});
  (*this->passwordSignText).setCharacterSize(31);
  (*this->passwordSignText).setFillColor(sf::Color::Black);

  this->passwordLine = new sf::RectangleShape({250, 33});
  (*this->passwordLine).setPosition({50, 114});

  this->passwordString = new sf::Text;
  (*this->passwordString).setFont(*this->menuFont);
  (*this->passwordString).setPosition({50, 107});
  (*this->passwordString).setCharacterSize(35);
  (*this->passwordString).setFillColor(sf::Color::Black);

  this->inputPassword = new sf::String;
  this->hidedPasswordString = new sf::String;
}

void start_menu::initLogin() {
  this->loginSign = new sf::RectangleShape({54, 25});
  (*this->loginSign).setPosition({25, 25});

  this->loginSignText = new sf::Text;
  (*this->loginSignText).setString("LOGIN:");
  (*this->loginSignText).setFont(*this->menuFont);
  (*this->loginSignText).setPosition({25, 18});
  (*this->loginSignText).setCharacterSize(31);
  (*this->loginSignText).setFillColor(sf::Color::Black);

  this->loginLine = new sf::RectangleShape({250, 33});
  (*this->loginLine).setPosition({50, 52});

  this->loginString = new sf::Text;
  (*this->loginString).setFont(*this->menuFont);
  (*this->loginString).setPosition({50, 45});
  (*this->loginString).setCharacterSize(35);
  (*this->loginString).setFillColor(sf::Color::Black);

  this->inputLogin = new sf::String;
}

sf::String &start_menu::getLoginString() { return *this->inputLogin; }

sf::Text &start_menu::getLoginText() { return *this->loginString; }

sf::String &start_menu::getPasswordString() { return *this->inputPassword; }

sf::Text &start_menu::getPasswordText() { return *this->passwordString; }

sf::String &start_menu::getHidedString() { return *this->hidedPasswordString; }

sf::Font &start_menu::getFont() { return *this->menuFont; }

start_menu::start_menu() { this->initMenu(); }

start_menu::~start_menu() {
  delete this->acceptButton;
  delete this->textButton;
  delete this->passwordSign;
  delete this->passwordSignText;
  delete this->passwordLine;
  delete this->inputPassword;
  delete this->passwordString;

  delete this->loginSign;
  delete this->loginSignText;
  delete this->loginLine;
  delete this->loginString;
  delete this->inputLogin;

  delete this->menuFont;
  delete this->hidedPasswordString;
}

void start_menu::renderMenu(sf::RenderWindow &win) {
  win.draw(*this->loginLine);
  win.draw(*this->loginSign);
  win.draw(*this->loginSignText);
  win.draw(*this->loginString);
  win.draw(*this->passwordLine);
  win.draw(*this->passwordSign);
  win.draw(*this->passwordSignText);
  win.draw(*this->passwordString);
  win.draw(*this->acceptButton);
  win.draw(*this->textButton);
}