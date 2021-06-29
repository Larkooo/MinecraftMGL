#pragma once

#include <memory>

#include <glad/glad.h>

#include "Window.h"
#include "World.h"

class Game
{
	std::unique_ptr<Window> m_Window = nullptr;
	World m_World;
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

