#include "include/page_lobby/page_lobby.h"
#include <iostream>

Page::Page(const sf::RenderWindow &window, std::size_t countOnPage_,
           const std::string &font_path, const sf::Vector2f &headCoords_,
           sf::Image *image, const sf::Vector2f &addLocation)
    : countOnPage(countOnPage_), pageNumber(0), headCoords(headCoords_) {
  addSession =
      std::make_unique<Button *>(new Button(image, image, addLocation));
  (*addSession)->setButtonScale(sf::Vector2f(1, 1));
  headButton.setButtonLocation(headCoords);
  usersBox = std::vector<std::unique_ptr<UserLobby *>>();
  if (!pageNumberFont.loadFromFile("include/512h/" + font_path)) {
    std::cerr << "error load font: [Page constructor]" << std::endl;
  }

  pageNumberText.setFont(pageNumberFont);
  pageNumberText.setString(std::to_string(pageNumber + 1));
  pageNumberText.setCharacterSize(24);
  pageNumberText.setFillColor(sf::Color::White);
  pageNumberText.setPosition(sf::Vector2f(
      window.getSize().x / 2 - pageNumberText.getLocalBounds().width,
      window.getSize().y - 2 * pageNumberText.getLocalBounds().height));
}
void Page::AddUserBox(std::unique_ptr<UserLobby *> &&userLobby) {
  if (usersBox.empty()) {
    usersBox.push_back(std::move(userLobby));
  } else {
    Button lastButton = (*usersBox.back())->getButton();
    sf::Text lastText = (*usersBox.back())->getText();
    (*userLobby)
        ->setButtonLocation(sf::Vector2f(lastButton.GetCoords().x,
                                         lastButton.GetCoords().y + 100));
    (*userLobby)
        ->setTextLocation(sf::Vector2f(lastText.getPosition().x,
                                       lastText.getPosition().y + 100));
    usersBox.push_back(std::move(userLobby));
  }
}
void Page::draw(sf::RenderWindow &window) {
  (*addSession)->draw(window);
  setPageButtons();
  std::size_t upButtonNumber = pageNumber * countOnPage;
  for (std::size_t i = upButtonNumber;
       i < std::min(usersBox.size(), (pageNumber + 1) * countOnPage); i++) {
    (*usersBox[i])->draw(window);
  }
  window.draw(pageNumberText);
}
void Page::clearPageButtons() {
  std::size_t upButtonNumber = pageNumber * countOnPage;
  for (std::size_t i = upButtonNumber;
       i < std::min(usersBox.size(), (pageNumber + 1) * countOnPage); i++) {
    (*usersBox[i])
        ->getButtonPointer()
        ->setButtonLocation(sf::Vector2f(1000, 1000));
  }
}
void Page::setPageButtons() {
  std::size_t upButtonNumber = pageNumber * countOnPage;
  sf::Vector2f buttonCoords = headButton.GetCoords();
  sf::Vector2f textCoords = buttonCoords;
  textCoords.x += 100;
  // textCoords.y += 50;

  for (std::size_t i = pageNumber * countOnPage;
       i < std::min(usersBox.size(), (pageNumber + 1) * countOnPage); i++) {
    buttonCoords.y += 100 * (i != upButtonNumber);
    textCoords.y += 100 * (i != upButtonNumber);
    (*usersBox[i])->setButtonLocation(buttonCoords);
    (*usersBox[i])->setTextLocation(textCoords);
  }
}
void Page::setHeadCoords(std::unique_ptr<Button *> &headButton_) {
  headButton = (**headButton_);
  headButton.setButtonLocation(headCoords);
  // (*headButton_)->setButtonLocation(headCoords);
}
void Page::update(sf::RenderWindow &window) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    std::cerr << "W" << std::endl;

    clearPageButtons();
    if (usersBox.empty()) {
      pageNumber = 0;
    } else {
      pageNumber =
          std::min((usersBox.size() - 1) / countOnPage, pageNumber + 1);
    }
    if (pageNumber * countOnPage < usersBox.size()) {
      setHeadCoords(
          (*usersBox[pageNumber * countOnPage])->getButtonUniquePtr());
    }
    // std::cerr << "buttonHead is " << pageNumber * countOnPage << std::endl;
    setPageButtons();

    pageNumberText.setString(std::to_string(pageNumber + 1));
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    std::cerr << "H" << std::endl;

    clearPageButtons();
    pageNumber = (!pageNumber ? 0 : pageNumber - 1);
    if (pageNumber * countOnPage < usersBox.size()) {
      setHeadCoords(
          (*usersBox[pageNumber * countOnPage])->getButtonUniquePtr());
    }
    setPageButtons();

    pageNumberText.setString(std::to_string(pageNumber + 1));
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    sf::Vector2i mouse_coords = sf::Mouse::getPosition(window);
    if (pageNumber * countOnPage < usersBox.size()) {
      std::vector<std::unique_ptr<UserLobby *>>::iterator userRemove =
          find_if(usersBox.begin() + (pageNumber * countOnPage), usersBox.end(),
                  [&](std::unique_ptr<UserLobby *> &usr) {
                    return (*usr)->getButtonPointer()->IsClicked(
                        sf::Vector2f(mouse_coords.x, mouse_coords.y));
                  });
      // std::cerr << "try to delete button which mustn't delete: "
      //           << (userRemove != std::end(usersBox) ? "oops" : "heey")
      //           << std::endl;
      bool isHeadDeleted = (userRemove != std::end(usersBox) &&
                            (**userRemove)->getButton() == headButton);
      if (userRemove != std::end(usersBox)) {
        usersBox.erase(
            std::remove(usersBox.begin(), usersBox.end(), *userRemove));
      }
      // std::cerr << "isHeadDeleted is " << isHeadDeleted << std::endl;
      clearPageButtons();
      if (isHeadDeleted) {
        // std::cerr << "remove head" << std::endl;
        if (usersBox.size() <= pageNumber * countOnPage) {
          pageNumber = (!pageNumber ? 0 : pageNumber - 1);
          pageNumberText.setString(std::to_string(pageNumber + 1));
        }
        if (usersBox.size() > pageNumber * countOnPage) {
          // std::cerr << "buttonHead now is " << pageNumber * countOnPage
          //           << std::endl;
          setHeadCoords(
              (*usersBox[pageNumber * countOnPage])->getButtonUniquePtr());
        }
      }
      setPageButtons();
    }
    if ((*addSession)
            ->IsClicked(sf::Vector2f(mouse_coords.x, mouse_coords.y))) {
      std::cerr << "I'm here" << std::endl;
      addSessionMenu(window);
      // tryingAddSession("here should be login"); // LOGINLOGINLOGIN
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
void Page::addSessionMenu(sf::RenderWindow &window) {
  std::cerr << "al;dfjas;lkdf" << std::endl;

  // sf::RenderWindow win(sf::VideoMode(1000, 1000), "what the fuck");
  // sf::RectangleShape rectangle(sf::Vector2f(200, 200));
  // rectangle.setSize(sf::Vector2f(600, 600));
  // rectangle.setFillColor(sf::Color(255, 0, 0));
  // rectangle.setPosition(200, 200);
  // sf::Image image;
  // if (!image.loadFromFile("backgroundSession.jpg")) {
  //   std::cerr << "error load jpg/png: [1 - addSessionMenu]" << std::endl;
  // }
  // sf::Texture texture;
  // if (!texture.loadFromImage(image)) {
  //   std::cerr << "error load from image: [2 - addSessionMenu]" << std::endl;
  // }
  // sf::Sprite sprite;
  // sprite.setTexture(texture);
  // // sprite.setTextureRect(sf::IntRect(40, 70, 640, 470));
  // sprite.setScale(4.8f, 3.2f);
  // sprite.setPosition(200, 200);

  sf::Image ppimage;
  if (!ppimage.loadFromFile("include/512h/Levelgreen-min-2.png")) {
    std::cerr << "error load jpg/png: [3 - addSessionMenu]" << std::endl;
  }
  sf::Image ppIimage;
  if (!ppIimage.loadFromFile("include/512h/Levelgreen-min-3.png")) {
    std::cerr << "error load jpg/png: [4 - addSessionMenu]" << std::endl;
  }
  // Button privateB(&ppIimage, &ppimage, sf::Vector2f(300, 300));
  // Button publicB(&ppIimage, &ppimage, sf::Vector2f(300, 400));
  std::vector<std::unique_ptr<Button *>> buttons;
  buttons.push_back(std::make_unique<Button *>(
      new Button(&ppIimage, &ppimage, sf::Vector2f(300, 300))));
  buttons.push_back(std::make_unique<Button *>(
      new Button(&ppIimage, &ppimage, sf::Vector2f(300, 400))));

  sf::Font font;
  font.loadFromFile("include/512h/font.ttf");

  sf::RectangleShape rectangle(sf::Vector2f(230, 40));
  rectangle.setPosition(sf::Vector2f(400, 305));
  rectangle.setFillColor(sf::Color(240, 240, 240, 50));

  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(41);
  text.setFillColor(sf::Color(255, 0, 0));
  text.setPosition(402, 300);
  std::string out = "";
  text.setString(out);

  sf::Image okImage;
  if (!okImage.loadFromFile("include/512h/OKbutton.png")) {
    std::cerr << "error load jpg/png: [5 - addSessionMenu]" << std::endl;
  }
  std::unique_ptr<Button *> okButton = std::make_unique<Button *>(
      new Button(&okImage, &okImage, sf::Vector2f(360, 500)));
  (*okButton)->setButtonScale(sf::Vector2f(1.5f, 1.5f));

  sf::Image backImage;
  if (!backImage.loadFromFile("include/512h/back.jpg")) {
    std::cerr << "error load image 3: [game]" << std::endl;
  }
  sf::Texture backTexture;
  if (!backTexture.loadFromImage(backImage)) {
    std::cerr << "error load from image = back: [game]" << std::endl;
  }
  sf::Sprite backSprite;
  backSprite.setTexture(backTexture);
  backSprite.setTextureRect(sf::IntRect(0, 0, 1000, 1000));
  backSprite.setPosition(0, 0);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::TextEntered) {
        if ((*buttons[0])->getPressed()) {
          if (event.text.unicode == '\b') {
            if (out.size()) {
              out.pop_back();
            }
          } else {
            out += event.text.unicode;
          }
          if (out.size() > 8) {
            out.pop_back();
          }

          std::string all_stars = "";
          for (std::size_t i = 0; i < out.size(); i++) {
            all_stars += "*";
          }
          text.setString(all_stars);
        }
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      break;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      sf::Vector2i mouse_coords = sf::Mouse::getPosition(window);
      for (std::size_t i = 0; i < buttons.size(); i++) {
        if ((*buttons[i])
                ->IsClicked(sf::Vector2f(mouse_coords.x, mouse_coords.y))) {
          (*buttons[i ^ 1])->setPressed(false);
          if (i) {
            rectangle.setFillColor(sf::Color(240, 240, 240, 50));
          } else {
            rectangle.setFillColor(sf::Color(240, 240, 240, 150));
          }
          break;
        }
      }

      if ((*okButton)->IsClicked(
              sf::Vector2f(mouse_coords.x, mouse_coords.y))) {
        tryingAddSession(
            "what the fuck?" +
            (std::vector<std::string>{"a", "b", "c", "d", "e"}[rand() % 5]));
        break;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    window.clear();
    window.draw(backSprite);
    // window.draw(sprite);
    window.draw(rectangle);
    window.draw(text);
    (*okButton)->draw(window);
    for (auto &but : buttons) {
      (*but)->draw(window);
    }
    window.display();
  }
}
void Page::tryingAddSession(const std::string &login) {
  sf::Image connectImage;
  if (!connectImage.loadFromFile("include/512h/Levelgreen-min-2.png")) {
    std::cerr << "error load image: [add_user]" << std::endl;
  }
  sf::Image backgroundImage;
  if (!backgroundImage.loadFromFile("include/512h/backgroundSession.png")) {
    std::cerr << "error load image 2: [add_user]" << std::endl;
  }
  AddUserBox(std::make_unique<UserLobby *>(
      new UserLobby(login,
                    std::make_unique<Button *>(new Button(
                        &connectImage, &connectImage, sf::Vector2f(0, 0))),
                    "Afont.ttf", 24, sf::Color(255, 0, 0), sf::Vector2f(150, 50),
                    &backgroundImage)));
}