#include "main_menu.h"

void main_menu::initButtons() {}

main_menu::main_menu() {
  this->mainMenuFont = new sf::Font;
  (*this->mainMenuFont).loadFromFile("include/512h/font1.ttf");
  this->gameButton = new sf::RectangleShape({500, 100});
  (*this->gameButton).setPosition({250, 200});
  this->gameButtonText = new sf::Text;
  (*this->gameButtonText).setString("Start game");
  (*this->gameButtonText).setFont(*this->mainMenuFont);
  (*this->gameButtonText).setFillColor(sf::Color::Black);
  (*this->gameButtonText).setPosition({250, 200});
  (*this->gameButtonText).setCharacterSize(39);
  this->optionsButton = new sf::RectangleShape({500, 100});
  (*this->optionsButton).setPosition({250, 310});
  this->optionsButtonText = new sf::Text;
  (*this->optionsButtonText).setString("Options");
  (*this->optionsButtonText).setFont(*this->mainMenuFont);
  (*this->optionsButtonText).setFillColor(sf::Color::Black);
  (*this->optionsButtonText).setPosition({250, 310});
  (*this->optionsButtonText).setCharacterSize(39);
  this->profileButton = new sf::RectangleShape({500, 100});
  (*this->profileButton).setPosition({250, 420});
  this->profileButtonText = new sf::Text;
  (*this->profileButtonText).setString("My profile");
  (*this->profileButtonText).setFont(*this->mainMenuFont);
  (*this->profileButtonText).setPosition({250, 420});
  (*this->profileButtonText).setFillColor(sf::Color::Black);
  (*this->profileButtonText).setCharacterSize(39);
  this->githubButton = new sf::RectangleShape({500, 100});
  (*this->githubButton).setPosition({250, 530});
  this->githubButtonText = new sf::Text;
  (*this->githubButtonText).setString("Check out our GitHub");
  (*this->githubButtonText).setFont(*this->mainMenuFont);
  (*this->githubButtonText).setFillColor(sf::Color::Black);
  (*this->githubButtonText).setPosition({250, 530});
  (*this->githubButtonText).setCharacterSize(39);
}

main_menu::~main_menu() {
  delete this->mainMenuFont;
  delete this->gameButtonText;
  delete this->optionsButton;
  delete this->optionsButtonText;
  delete this->profileButton;
  delete this->profileButtonText;
  delete this->githubButton;
  delete this->githubButtonText;
}

void main_menu::render(sf::RenderWindow &win) {
  win.draw(*this->gameButton);
  win.draw(*this->gameButtonText);
  win.draw(*this->optionsButton);
  win.draw(*this->optionsButtonText);
  win.draw(*this->profileButton);
  win.draw(*this->profileButtonText);
  win.draw(*this->githubButton);
  win.draw(*this->githubButtonText);
}
