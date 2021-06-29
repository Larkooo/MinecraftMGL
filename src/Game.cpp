#include "Core/Game.h"

Game::Game(const Window& window) : m_Window(std::make_unique<Window>(window))
{
}

Game::~Game()
{
	Clean();
}

void Game::Init()
{
	if (not glfwInit())
	{
		std::cout << "Could not initialize glfw" << std::endl;
		return;
	}

	// initialize the window and context
	m_Window->Init();

	m_Running = true;
}

void Game::HandleEvents()
{
}

void Game::Update()
{
}

void Game::Render()
{
}

void Game::Clean()
{
}



/*Game::Game(Window&& window) : m_Window(std::make_unique<Window>(window))
{
	if (not glfwInit())
	{
		std::cout << "Could not initialize glfw" << std::endl;
		return;
	}

	// initialize the window and context
	m_Window->Init();
}*/