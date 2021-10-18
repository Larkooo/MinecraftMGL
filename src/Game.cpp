#include "Core/Game.h"

#include "Core/World.h"

Game* Game::sGame = nullptr;

Game::Game(Window&& window) : m_Window(std::move(window))
{
	m_World = std::make_unique<World>();
}

Game::Game(const Window& window) : m_Window(window)
{
	m_World = std::make_unique<World>();
}


Game::~Game()
{
	Clean();
}

void Game::Init()
{
	if (!glfwInit())
	{
		std::cout << "Could not initialize glfw" << std::endl;
		return;
	}

	// Set "assertions", if those checks are false, opengl will not render anything
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// initialize the window and context
	m_Window.Init();

	glfwSetInputMode(m_Window.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);

	Game::sGame = this;

	auto resizeCallback = [](GLFWwindow* window, int width, int height)
	{
		Game::Instance()->GetWindow().SetWidth(width);
		Game::Instance()->GetWindow().SetHeight(width);
		glViewport(0, 0, width, height);
	};
	glfwSetFramebufferSizeCallback(m_Window.GetGLFWWindow(), resizeCallback);

	glActiveTexture(GL_TEXTURE0);
	m_TextureMap = std::make_unique<Texture>("./res/textures/texturemap_64x64.png");

	m_World->Init();
	// Create the projection matrix using the player fov
	m_Projection = glm::perspective(
		glm::radians(m_World->GetPlayer().GetCamera().GetFov()), 
		(float)m_Window.GetWidth() / m_Window.GetHeight(), 
		m_World->GetPlayer().GetCamera().GetNear(), m_World->GetPlayer().GetCamera().GetFar());

	m_Now = std::chrono::high_resolution_clock::now();
	m_Running = true;
}

Game* Game::Instance()
{
	if (sGame == nullptr)
		sGame = new Game;
	return sGame;
}

void Game::HandleEvents()
{
	m_Last = m_Now;
	m_Now = std::chrono::high_resolution_clock::now();
	m_DeltaTime = m_Now - m_Last;

	if (glfwGetKey(m_Window.GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window.GetGLFWWindow(), true);

	if (glfwWindowShouldClose(m_Window.GetGLFWWindow()))
		m_Running = false;

	m_World->HandleEvents();
}

void Game::Update()
{
	m_World->Update();
}

void Game::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_World->Render();

	glfwSwapBuffers(m_Window.GetGLFWWindow());
	glfwPollEvents();
}

void Game::Clean()
{
	glfwTerminate();
}