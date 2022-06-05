#include "include/game/game.h"
#include <Graphics.hpp>
#include <filesystem>
// #include "include/pqxx/pqxx"
#include <fstream>
#include <iostream>

void run_gui() {
  std::cout << std::filesystem::current_path() << std::endl;
  // game runned_game;
  // while (runned_game.isRunning()) {
  //   runned_game.update();
  //   runned_game.render();
  // }

  // std::ifstream in("src/a.txt");
  // std::string s;
  // in >> s;
  // std::cerr << s << std::endl;

  sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");
  // sf::RectangleShape rectangle(sf::Vector2f(100, 100));
  // rectangle.setPosition(sf::Vector2f(50, 50));
  // rectangle.setFillColor(sf::Color::White);

  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(30);
  window.setActive();
  sf::Image image;
  if (!image.loadFromFile(
          "src/bb.png")) { // /home/god/HSE/course_1/PROJECT_TO_GOOGLE/"
                           // "HSE_PROJECT_online_chess/src/bb.png"
    std::cerr << "error load image" << std::endl;
  }
  sf::Texture texture;
  if (!texture.loadFromImage(image)) {
    std::cerr << "error load texture" << std::endl;
  }
  sf::Sprite sprite;
  sprite.setTexture(texture);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    window.draw(sprite);
    // window.draw(rectangle);
    window.display();
  }
}

int main() {
  run_gui();
  return 0;
}