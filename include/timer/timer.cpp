#include "include/timer/timer.h"

Timer::Timer(const std::string &button_path, const std::string &font_path,
             std::size_t seconds_, const sf::Vector2f &coords)
    : seconds(seconds_), speed(1) {
  sf::Image button_image;
  if (!button_image.loadFromFile("include/512h/" + button_path)) {
    std::cerr << "error loadFromFile image: [timer constructor]" << std::endl;
  }
  clock = std::make_unique<Button *>(
      new Button(&button_image, &button_image, coords));
  (*clock)->setButtonScale(sf::Vector2f(1.3f, 1.3f));
  text = std::make_unique<Text *>(new Text(font_path, 65, coords));
}
std::string Timer::getGameTime() {
  std::string tm = "";
  std::string min = std::to_string(seconds / 60);
  tm += (min.size() == 1 ? "0" + min : min);
  std::string sec = std::to_string(seconds % 60);
  tm += ":";
  tm += (sec.size() == 1 ? "0" + sec : sec);

  return tm;
}
void Timer::draw(sf::RenderWindow &window) {
  (*clock)->draw(window);
  (*text)->setTextString(getGameTime());
  (*text)->draw(window);
}
void Timer::update() {
  if (speed > seconds) {
    seconds = 0;
  } else {
    seconds -= speed;
  }
}
void Timer::stop() { speed = 0; }
std::size_t Timer::getSeconds() const { return seconds; }