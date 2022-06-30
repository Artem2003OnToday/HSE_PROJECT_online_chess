#include "include/button/button.h"
#include "include/game/game.h"
#include "include/page_lobby/page_lobby.h"
#include "include/timer/timer.h"
#include <Graphics.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
// #include "include/pqxx/pqxx"

const std::size_t MAX_PAGE_SIZE = 6;
const int WIDTH = 1000;
const int HEIGHT = 1000;

void run_Artem_gui() {
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);
  window.setActive();

  sf::Image back;
  if (!back.loadFromFile("include/512h/back.jpg")) {
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

  if (!image.loadFromFile("include/512h/Levelgreen-min-2.png")) {
    std::cerr << "error load image: [game]" << std::endl;
  }
  Page pages(window, MAX_PAGE_SIZE, "Afont.ttf", sf::Vector2f(100, 200), &image,
             sf::Vector2f(5, 5));

  Timer timer("button.png", "Afont.ttf", 120, sf::Vector2f(300, 300));
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(sprite);
    timer.draw(window);
    timer.update();
    // timer.update();
    // pages.draw(window);
    // pages.update(window);

    window.display();
  }
}

void run_Kirill_gui() {
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "chess");
  window.setActive();
  game runned_game;
  while (runned_game.isRunning(window)) {
    runned_game.render(window);
    runned_game.update(window);
  }
}

// void client_server_part() {
//   GameClient guide(grpc::CreateChannel("localhost:50051",
//                                        grpc::InsecureChannelCredentials()));
//   current_session_token = guide.Auth("login", "password");
//   std::cout << current_session_token << std::endl;
//   guide.StartGame();

//   while (1) {
//     guide.MakeTurn();
//     while (!guide.IsAlive()) {
//       std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     }
//     std::cout << "wow" << std::endl;
//   }
// }

int main() {
  run_Kirill_gui();
  // client_server_part();
  // run_Artem_gui();
  return 0;
}