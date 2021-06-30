#include "Core/Window.h"

Window::Window(const Window& other)
{
	memcpy(this, &other, sizeof(other));
}

Window::Window(Window&& other)
{
	memcpy(this, &other, sizeof(other));
	other.m_Window = nullptr;
	std::cout << "Moved" << std::endl;
}

Window::~Window()
{
	std::cout << "Destroyed" << std::endl;
	glfwDestroyWindow(m_Window);
}

void Window::Init()
{
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

	if (m_Window == nullptr)
	{
		glfwTerminate();
		std::cout << "Could not create window!" << std::endl;
		return;
	}
	
	glfwMakeContextCurrent(m_Window);

	if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return;
	}
		
	if (m_VSync) glfwSwapInterval(1);
}

i32 Window::GetWidth()
{
	return m_Width;
}

i32 Window::GetHeight()
{
	return m_Height;
}

void Window::SetVSync(bool state)
{
	if (state)
	{
		glfwSwapInterval(1);
		m_VSync = true;
	}
	else
	{
		glfwSwapInterval(0);
		m_VSync = false;
	}
}

bool Window::VSyncEnabled()
{
	return m_VSync;
}