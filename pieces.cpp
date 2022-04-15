#include "pieces.h"

pieces::~pieces()
{
	delete this->pieceImage;
	delete this->pieceSprite;
	delete this->pieceTexture;
}


sf::Sprite& pieces::getPieceMadel()
{
	return *this->pieceSprite;
}

sf::Sprite& pieces::setTexture(const std::string& file)
{
	this->pieceImage = new sf::Image;
	this->pieceSprite = new sf::Sprite;
	this->pieceTexture = new sf::Texture;
	(*this->pieceImage).loadFromFile(file);
	(*this->pieceTexture).loadFromImage(*this->pieceImage);
	(*this->pieceSprite).setTexture(*this->pieceTexture);
	(*this->pieceSprite).setScale(sf::Vector2f(0.18, 0.18));
	return *this->pieceSprite;
}

pieces::operator bool()
{
	return this->team;
}

void pieces::nullSpecial()
{
	this->specialMove = false;
}

bool pieces::getSpecial()
{
	return this->specialMove;
}

int pieces::getPieceType()
{
	return this->type;
}

std::vector<sf::Vector2i> pieces::getMovements()
{
	return this->movements;
}

pieces::pieces(std::vector<sf::Vector2i> moves, int pieceCost, bool special, bool pieceTeam, std::string file, int pieceType)
{
	this->movements = moves;
	this->cost = pieceCost;
	this->specialMove = special;
	this->team = pieceTeam;
	this->setTexture(file);
	this->type = pieceType;
}

