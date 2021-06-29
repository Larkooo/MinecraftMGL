#include <Core/Game.h>

int main()
{
	Window window;
	Game game(window);

	while (game.IsRunning())
	{
		game.HandleEvents();
		game.Update();
		game.Render();
	}

	game.Clean();

	std::cin.get();
	return 0;
}