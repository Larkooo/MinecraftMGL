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

	// Set hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// initialize the window and context
	m_Window->Init();

	m_Running = true;
}

void Game::HandleEvents()
{
	glfwPollEvents();
}

void Game::Update()
{

}

void Game::Render()
{
	glfwSwapBuffers(m_Window->GetWindow());
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