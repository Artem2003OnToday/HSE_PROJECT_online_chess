#include "include/user_lobby/user_lobby.h"
#include <iostream>

UserLobby::UserLobby(const std::string &userName,
                     std::unique_ptr<Button *> &&button,
                     const std::string &font_path, std::size_t characterSize,
                     const sf::Color &textColor,
                     const sf::Vector2f &textPosition,
                     sf::Image *backgroundImage)
    : userName(userName), startGameButton(std::move(button)) {
  if (!font.loadFromFile("include/512h/" + font_path)) {
    std::cerr << "error load font: [UserLobby constructor]" << std::endl;
  }
  text.setFont(font);
  text.setString(userName);
  text.setCharacterSize(characterSize);
  text.setFillColor(textColor);
  text.setPosition(textPosition);
  setBackground(backgroundImage);
}
void UserLobby::draw(sf::RenderWindow &window) {
  (**background).draw(window);
  (**startGameButton).draw(window);
  window.draw(text);
}
Button UserLobby::getButton() const {
  Button button = **startGameButton;
  return button;
}
sf::Text UserLobby::getText() const {
  sf::Text text_copy = text;
  return text_copy;
}
void UserLobby::setButtonLocation(const sf::Vector2f &location) {
  (*startGameButton)->setButtonLocation(location);
  (*background)
      ->setButtonLocation(sf::Vector2f(location.x - 10, location.y - 15));
}
void UserLobby::setTextLocation(const sf::Vector2f &location) {
  text.setPosition(location);
}
Button *UserLobby::getButtonPointer() { return *startGameButton; }
void UserLobby::setTextShift(const sf::Vector2f &location) {
  text.setPosition(text.getPosition().x, text.getPosition().y - location.y);
}
sf::Vector2f UserLobby::getTextLocation() const { return text.getPosition(); }
std::unique_ptr<Button *> &UserLobby::getButtonUniquePtr() {
  return startGameButton;
}
void UserLobby::setBackground(sf::Image *image) {
  sf::Vector2f coords = (**startGameButton).GetCoords();
  background = std::make_unique<Button *>(
      new Button(image, image, sf::Vector2f(coords.x - 20, coords.y - 20)));
  (*background)->setButtonScale(sf::Vector2f(4.0f, 1.9f));
}