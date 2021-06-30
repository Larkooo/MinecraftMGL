#pragma once

#include <memory>

#include <glad/glad.h>

#include "Window.h"
#include "World.h"

class Game
{
	Window m_Window;
	World m_World;
	bool m_Running = false;

	static Game* sGame;
public:
	Game(const Window& window = Window());
	Game(Window&& window);
	Game(const Game&) = delete;
	~Game();	

	static Game* Instance();

	Window& GetWindow() { return m_Window; }
	bool IsRunning() { return m_Running; }

	void Init();
	
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
};

