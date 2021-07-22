#include "Core/Window.h"

Window::Window(const Window& other)
{
	m_Title = other.m_Title;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_VSync = other.m_VSync;
	m_Window = other.m_Window;

	std::cout << "Copied window" << std::endl;
}

Window::Window(Window&& other)
{
	m_Title = other.m_Title;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_VSync = other.m_VSync;
	m_Window = other.m_Window;

	other.m_Window = nullptr;
	std::cout << "Moved window" << std::endl;
}

Window::~Window()
{
	std::cout << "Destroyed window" << std::endl;
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