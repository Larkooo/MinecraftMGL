#include <Core/Game.h>

int main()
{
	Game* game = new Game(Window("Window", 800, 600));

	game->Init();

	while (game->IsRunning())
	{
		game->HandleEvents();
		game->Update();
		game->Render();
	}

	game->Clean();

	return 0;
}