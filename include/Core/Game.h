#pragma once

#include <memory>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Graphics/Texture.h"

class World;

#include <Definitions.h>
#include <glm/glm.hpp>

class Game
{
	Window m_Window;
	std::unique_ptr<World> m_World = nullptr;
	std::unique_ptr<Texture> m_TextureMap = nullptr;
	bool m_Running = false;

	// Rendering
	glm::mat4 m_Projection;

	// Clock
	std::chrono::steady_clock::time_point m_Now;
	std::chrono::steady_clock::time_point m_Last;

	std::chrono::duration<float> m_DeltaTime;

private:
	static Game* sGame;

public:
	static Game* Instance();

public:
	Game(const Window& window = Window());
	Game(Window&& window);
	Game(const Game&) = delete;
	~Game();	

	World& GetWorld() { return *m_World; }
	Window& GetWindow() { return m_Window; }
	glm::mat4 GetProjection() const { return m_Projection; }
	std::chrono::duration<float> GetDeltaTime() const { return m_DeltaTime; }
	Texture& GetTextureMap() { return *m_TextureMap; }

	bool IsRunning() { return m_Running; }

	void Init();
	
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
};

