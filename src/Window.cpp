#include "Core/Window.h"

void Window::Init()
{
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
}
