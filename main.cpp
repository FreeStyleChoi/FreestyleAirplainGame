#include "Game.h"

int main(int argc, char** argv)
{
	Game game;

	game.Init();
	do
	{
		game.Setting();
		game.Update();
	} while (game.restart);
	game.Finalize();

	return 0;
}