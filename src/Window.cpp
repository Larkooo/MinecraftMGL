#include "Core/Window.h"

Window::Window(const Window& other)
{
	m_Title = other.m_Title;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_VSync = other.m_VSync;
	m_Window = other.m_Window;
}

Window::Window(Window&& other) noexcept
{
	m_Title = other.m_Title;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_VSync = other.m_VSync;
	m_Window = other.m_Window;

	other.m_Window = nullptr;
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

void Window::Init()
{
	if (m_Window != nullptr)
		return;

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

	if (m_Window == nullptr)
	{
		glfwTerminate();
		std::cout << "Could not create window!" << std::endl;
		return;
	}
	
	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return;
	}
		
	if (m_VSync) glfwSwapInterval(1);
}

i32 Window::GetWidth() const
{
	return m_Width;
}

i32 Window::GetHeight() const
{
	return m_Height;
}

void Window::SetVSync(bool state)
{
	glfwSwapInterval(state);
	m_VSync = state;
}

bool Window::VSyncEnabled() const
{
	return m_VSync;
}

Window& Window::operator=(const Window& other)
{
	if (this == &other)
		return *this;

	m_Title = other.m_Title;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_VSync = other.m_VSync;

	glfwSetWindowTitle(m_Window, m_Title.c_str());
	glfwSetWindowSize(m_Window, m_Width, m_Height);
	glfwSwapInterval(m_VSync);

	return *this;
}

Window& Window::operator=(Window&& other) noexcept
{
	if (this == &other)
		return *this;

	this->~Window();

	m_Title = other.m_Title;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_VSync = other.m_VSync;
	m_Window = other.m_Window;

	other.m_Window = nullptr;

	return *this;
}
