#include "include/button/button.h"
#include <iostream>

void Button::initSprite(sf::Sprite *sprite, sf::Texture *texture,
                        sf::Image *image) {
  if (image == nullptr) {
    std::cerr << "image is nullptr!" << std::endl;
    return;
  }
  image->createMaskFromColor(sf::Color(71, 112, 76));
  if (!texture->loadFromImage(*image)) {
    std::cerr << "error load texture from image: [Button]" << std::endl;
  }
  sprite->setTexture(*texture);
}

Button::Button(sf::Image *notPressedImage_, sf::Image *pressedImage_,
               const sf::Vector2f &coords_)
    : coords(coords_), notPressedImage(*notPressedImage_),
      pressedImage(*pressedImage_), press(false) {
  initSprite(&notPressedSprite, &notPressedTexture, &notPressedImage);
  initSprite(&pressedSprite, &pressedTexture, &pressedImage);
  notPressedSprite.setPosition(coords);
  pressedSprite.setPosition(coords);

  notPressedSprite.setScale(0.3f, 0.3f);
  pressedSprite.setScale(0.3f, 0.3f);
}

bool Button::IsClicked(const sf::Vector2f &currentMouseLocation) {
  // std::cerr << currentMouseLocation.x << ' ' << currentMouseLocation.y
  //           << std::endl;
  bool click =
      notPressedSprite.getGlobalBounds().contains(currentMouseLocation);
  press ^= click;
  return click;
}
bool Button::getPressed() const { return press; }
void Button::setPressed(bool mfalse) { press = mfalse; }

sf::Sprite Button::GetNotPressedSprite() const { return notPressedSprite; }
sf::Vector2f Button::GetCoords() const {
  return notPressedSprite.getPosition();
}
int Button::GetWidth() const { return notPressedSprite.getLocalBounds().width; }
int Button::GetHeight() const {
  return notPressedSprite.getLocalBounds().height;
}
void Button::draw(sf::RenderWindow &window) {
  if (!press) {
    window.draw(notPressedSprite);
  } else {
    window.draw(pressedSprite);
  }
}
void Button::setButtonLocation(const sf::Vector2f &location) {
  notPressedSprite.setPosition(location);
  pressedSprite.setPosition(location);
}
void Button::setButtonShift(const sf::Vector2f &location) {
  notPressedSprite.setPosition(GetCoords().x - location.x,
                               GetCoords().y - location.y);
  pressedSprite.setPosition(GetCoords().x - location.x,
                            GetCoords().y - location.y);
}

bool Button::operator==(const Button &button) const {
  return notPressedSprite.getPosition() == button.GetCoords();
}

void Button::setButtonScale(const sf::Vector2f &scale) {
  notPressedSprite.setScale(scale);
  pressedSprite.setScale(scale);
}