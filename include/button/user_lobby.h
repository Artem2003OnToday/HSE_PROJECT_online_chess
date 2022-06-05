#ifndef LOBBY_H_
#define LOBBY_H_

#include "button.h"

class UserLobby {
private:
  std::string userName;
  std::unique_ptr<Button *> startGameButton;
  std::unique_ptr<Button *> background;
  sf::Font font;
  sf::Text text;

public:
  UserLobby() = default;
  UserLobby(const std::string &userName, std::unique_ptr<Button *> &&button,
            const std::string &font_path, std::size_t characterSize,
            const sf::Color &textColor, const sf::Vector2f &textPosition,
            sf::Image *backgroundImage);
  void draw(sf::RenderWindow &window);
  Button getButton() const;
  sf::Text getText() const;
  Button *getButtonPointer();
  sf::Vector2f getTextLocation() const;
  void setTextShift(const sf::Vector2f &location);
  void setButtonLocation(const sf::Vector2f &location);
  void setTextLocation(const sf::Vector2f &location);
  void setBackground(sf::Image *image);
  std::unique_ptr<Button *> &getButtonUniquePtr();
};

#endif // LOBBY_H_