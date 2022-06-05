#ifndef BUTTON_H_
#define BUTTON_H_

#include <SFML/Graphics.hpp>

class Button {
private:
  sf::Sprite notPressedSprite;
  sf::Sprite pressedSprite;
  sf::Texture notPressedTexture;
  sf::Texture pressedTexture;
  sf::Image notPressedImage;
  sf::Image pressedImage;
  sf::Vector2f coords;
  bool press;

public:
  static void initSprite(sf::Sprite *sprite, sf::Texture *texture,
                         sf::Image *image);
  Button() = default;
  Button(sf::Image *notPressedImage_, sf::Image *pressedImage_,
         const sf::Vector2f &coords_);
  bool IsClicked(const sf::Vector2f &currentMouseCoords);
  int GetWidth() const;
  int GetHeight() const;
  void draw(sf::RenderWindow &window);
  void setButtonLocation(const sf::Vector2f &location);
  void setButtonShift(const sf::Vector2f &location);
  sf::Sprite GetNotPressedSprite() const;
  sf::Vector2f GetCoords() const;
  void setButtonScale(const sf::Vector2f &scale);
  bool getPressed() const;
  void setPressed(bool mfalse);
  bool operator==(const Button &button) const;
};

#endif // BUTTON_H_