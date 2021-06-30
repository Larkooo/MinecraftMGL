#include "Core/Game.h"
#include <glm/glm.hpp>

#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/Shader.h>

Game* Game::sGame = nullptr;

Game::Game(Window&& window) : m_Window(std::move(window))
{
}

Game::Game(const Window& window) : m_Window(window)
{
	//std::cout << &m_World << std::endl;
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

	// Set "assertions", if those checks are false, opengl will not render anything
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// initialize the window and context
	m_Window.Init();

	glViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());

	Game::sGame = this;

	auto resizeCallback = [](GLFWwindow* window, int width, int height) {
		Game::Instance()->GetWindow().SetWidth(width);
		Game::Instance()->GetWindow().SetHeight(width);
		glViewport(0, 0, width, height);
	};
	glfwSetFramebufferSizeCallback(m_Window.GetGLFWWindow(), resizeCallback);

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
}

void Game::Update()
{

}

void Game::Render()
{
	glm::vec2 vertices[] = {
		 glm::vec2(0.5f, 0.5f),
		 glm::vec2(-0.5f, -0.5f),
		 glm::vec2(0.5f, -0.5f)
	};
	float fVertices[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		  0.0f,  0.5f, 0.0f
	};

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	VertexArray vao;
	vao.Bind();
	VertexBuffer vbo(fVertices, sizeof(fVertices));
	vbo.Bind();

	Shader shader("./res/shaders/triangle.vert", "./res/shaders/triangle.frag");
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	vao.Unbind();
	
	vao.Bind();

	shader.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(m_Window.GetGLFWWindow());
	glfwPollEvents();
}

void Game::Clean()
{
	glfwTerminate();
}