#pragma once

#include <memory>

#include "Window.h"

class Game
{
	std::unique_ptr<Window> m_Window = nullptr;
	bool m_Running;

public:
	Game(const Window& window);
	~Game();
	//Game(Window* window);
	//Game(Window&& window);

	bool IsRunning() { return m_Running; }

	void Init();
	
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
};

