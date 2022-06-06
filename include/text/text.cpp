#include "include/text/text.h"

Text::Text(const std::string &font_path, std::size_t characterSize_,
           const sf::Vector2f &coords)
    : characterSize(characterSize_) {
  if (!font.loadFromFile("include/512h/" + font_path)) {
    std::cerr << "error loadFromFile font: [text constructor]" << std::endl;
  }
  text.setFont(font);
  text.setCharacterSize(characterSize);
  text.setPosition(coords);
  text.setFillColor(sf::Color::White);
}
void Text::draw(sf::RenderWindow &window) { window.draw(text); }
void Text::setTextString(const std::string &new_text) {
  textString = new_text;
  text.setString(textString);
}
std::string Text::getTextString() const { return textString; }
