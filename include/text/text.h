#ifndef TEXT_H_
#define TEXT_H_

#include <Graphics.hpp>
#include <iostream>

class Text {
private:
  sf::Font font;
  sf::Text text;
  std::string textString;
  std::size_t characterSize;

public:
  Text() = default;
  Text(const std::string &font_path, std::size_t characterSize_,
       const sf::Vector2f &coords);
  void draw(sf::RenderWindow &window);
  void setTextString(const std::string &new_text);
  std::string getTextString() const;
};

#endif // TEXT_H_