#include <Core/Game.h>

int main()
{
	Game game(Window("Window", 1280, 720));

	game.Init();

	while (game.IsRunning())
	{
		game.HandleEvents();
		game.Update();
		game.Render();
	}

	game.Clean();

	return 0;
}