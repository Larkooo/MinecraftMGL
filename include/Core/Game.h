#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

class World;

#include <Definitions.h>
#include <glm/glm.hpp>

class Game
{
	Window m_Window;
	std::unique_ptr<World> m_World = nullptr;
	bool m_Running = false;

	// Rendering
	glm::mat4 m_Projection;

	static Game* sGame;
public:
	Game(const Window& window = Window());
	Game(Window&& window);
	Game(const Game&) = delete;
	~Game();	

	static Game* Instance();

	World& GetWorld() { return *m_World; }
	Window& GetWindow() { return m_Window; }
	glm::mat4 GetProjection() { return m_Projection; }
	bool IsRunning() { return m_Running; }

	void Init();
	
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
};

