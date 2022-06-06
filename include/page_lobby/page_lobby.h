#ifndef PAGE_LOBBY_H_
#define PAGE_LOBBY_H_

#include "include/user_lobby/user_lobby.h"
#include <Graphics.hpp>
#include <Window.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

class Page {
private:
  std::vector<std::unique_ptr<UserLobby *>> usersBox;
  std::unique_ptr<Button *> addSession;
  std::size_t countOnPage;
  std::size_t pageNumber;
  sf::Text pageNumberText;
  sf::Font pageNumberFont;
  Button headButton;
  sf::Vector2f headCoords;

public:
  Page() = default;
  Page(const sf::RenderWindow &window, std::size_t countOnPage_,
       const std::string &font_path, const sf::Vector2f &headCoords_,
       sf::Image *image, const sf::Vector2f &addLocation);
  void AddUserBox(std::unique_ptr<UserLobby *> &&userLobby);
  void draw(sf::RenderWindow &window);
  void update(sf::RenderWindow &window_);
  void clearPageButtons();
  void setPageButtons();
  void setHeadCoords(std::unique_ptr<Button *> &headButton_);
  void setAddSessionButton(sf::Image *image, const sf::Vector2f &location);
  void addSessionMenu(sf::RenderWindow &window);
  void tryingAddSession(const std::string &login);
};

#endif // PAGE_LOBBY_H_