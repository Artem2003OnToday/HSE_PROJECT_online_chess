#include "include/piece_upgrade/piece_upgrade.h"

piece_upgrade::piece_upgrade() {
  this->pieceImageList.resize(4);
  this->pieceTextureList.resize(4, new sf::Texture);
  this->pieceSpriteList.resize(4, new sf::Sprite);
  for (auto &piece : this->pieceSpriteList) {
    piece = new sf::Sprite;
  }
  for (auto &piece : this->pieceImageList) {
    piece = new sf::Image;
  }
  for (auto &piece : this->pieceTextureList) {
    piece = new sf::Texture;
  }
  this->pieceImageList[0]->loadFromFile("include/512h/br.png");
  this->pieceImageList[1]->loadFromFile("include/512h/bk.png");
  this->pieceImageList[2]->loadFromFile("include/512h/bb.png");
  this->pieceImageList[3]->loadFromFile("include/512h/bq.png");
  this->pieceTextureList[0]->loadFromImage(*this->pieceImageList[0]);
  this->pieceTextureList[1]->loadFromImage(*this->pieceImageList[1]);
  this->pieceTextureList[2]->loadFromImage(*this->pieceImageList[2]);
  this->pieceTextureList[3]->loadFromImage(*this->pieceImageList[3]);
  this->pieceSpriteList[0]->setTexture(*this->pieceTextureList[0]);
  this->pieceSpriteList[1]->setTexture(*this->pieceTextureList[1]);
  this->pieceSpriteList[2]->setTexture(*this->pieceTextureList[2]);
  this->pieceSpriteList[3]->setTexture(*this->pieceTextureList[3]);
  this->pieceSpriteList[0]->setScale(0.2f, 0.2f);
  this->pieceSpriteList[1]->setScale(0.2f, 0.2f);
  this->pieceSpriteList[2]->setScale(0.2f, 0.2f);
  this->pieceSpriteList[3]->setScale(0.2f, 0.2f);
  this->pieceSpriteList[0]->setPosition(300, 0);
  this->pieceSpriteList[1]->setPosition(400, 0);
  this->pieceSpriteList[2]->setPosition(500, 0);
  this->pieceSpriteList[3]->setPosition(600, 0);
  this->pieceButton = new sf::RectangleShape({400, 100});
  this->pieceButton->setPosition(300, 0);
  this->pieceButton->setFillColor(sf::Color(128, 128, 128));
}

piece_upgrade::~piece_upgrade() {
  for (auto &piece : this->pieceSpriteList) {
    delete piece;
  }
  for (auto &piece : this->pieceImageList) {
    delete piece;
  }
  for (auto &piece : this->pieceTextureList) {
    delete piece;
  }
  delete this->pieceButton;
}

void piece_upgrade::renderUpgrade(sf::RenderWindow &window) {
  window.draw(*this->pieceButton);
  for (auto &piece : this->pieceSpriteList) {
    window.draw(*piece);
  }
}