#include "button.h"
#include "page_lobby.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

const std::size_t MAX_PAGE_SIZE = 6;
const int WIDTH = 1000;
const int HEIGHT = 1000;

void add_user(Page &pages, const std::string &user_name) {
  sf::Image connectImage;
  if (!connectImage.loadFromFile("Levelgreen-min-2.png")) {
    std::cerr << "error load image: [add_user]" << std::endl;
  }
  sf::Image backgroundImage;
  if (!backgroundImage.loadFromFile("backgroundSession.png")) {
    std::cerr << "error load image 2: [add_user]" << std::endl;
  }

  pages.AddUserBox(std::make_unique<UserLobby *>(
      new UserLobby(user_name,
                    std::make_unique<Button *>(new Button(
                        &connectImage, &connectImage, sf::Vector2f(0, 0))),
                    "font.ttf", 24, sf::Color(255, 0, 0), sf::Vector2f(150, 50),
                    &backgroundImage)));
}

void game() {
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  sf::Image back;
  if (!back.loadFromFile("back.jpg")) {
    std::cerr << "error load image 3: [game]" << std::endl;
  }
  sf::Texture texture;
  if (!texture.loadFromImage(back)) {
    std::cerr << "error load from image = back: [game]" << std::endl;
  }
  sf::Sprite sprite;
  sprite.setTexture(texture);
  sprite.setTextureRect(sf::IntRect(0, 0, 1000, 1000));
  sprite.setPosition(0, 0);

  sf::Image image;
  if (!image.loadFromFile("Levelgreen-min-2.png")) {
    std::cerr << "error load image: [game]" << std::endl;
  }
  Page pages(window, MAX_PAGE_SIZE, "font.ttf", sf::Vector2f(100, 200), &image,
             sf::Vector2f(5, 5));

  //   for (int i = 0; i < 12; i++) {
  //     add_user(pages, "user" + std::to_string(i));
  //   }
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(sprite);
    pages.draw(window);
    pages.update(window);

    window.display();
  }
}

int main() {
  srand(time(NULL));
  game();
  return 0;
}