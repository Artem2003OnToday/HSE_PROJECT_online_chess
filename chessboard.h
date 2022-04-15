#pragma once
#include<vector>
#include"chessboard_square.h"

void changeColor(std::string& file);

class chessboard
{
private:
	std::vector<std::vector<chessboard_square*>> squares;
	void initBoard();
	void initVaraible();
	void initPieces();
public:
	chessboard();
	~chessboard();
	void renderBoard(sf::RenderWindow& win);
	chessboard_square& getSquare(sf::Vector2i coordinates);
	void movePiece(sf::Vector2i oldSquare,sf::Vector2i newSquare);
	void cleanMark();
};

