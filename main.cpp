#include "game.h"

int main()
{
	game runned_game;

	while (runned_game.isRunning())
	{

		runned_game.update();

		runned_game.render();

	}

	return 0;
}