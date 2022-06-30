#ifndef TIMER_H_
#define TIMER_H_

#include "include/button/button.h"
#include "include/text/text.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

class Timer {
private:
  std::unique_ptr<Button *> clock;
  std::unique_ptr<Text *> text;
  std::size_t seconds;
  int speed;
  std::mutex mu_;

public:
  Timer() = default;
  Timer(const std::string &button_path, const std::string &font_path,
        std::size_t seconds_, const sf::Vector2f &coords);
  void draw(sf::RenderWindow &window);
  void update();
  void stop();
  std::string getGameTime();
  std::size_t getSeconds() const;
};

#endif // TIMER_H_