#pragma once

#include <Definitions.h>
#include <string>

#include <GLFW/glfw3.h>

class Window
{
	std::string m_Title;
	i32 m_Width;
	i32 m_Height;
	bool m_VSync;

	GLFWwindow* m_Window = nullptr;

public:
	Window(const std::string& title = "Window", i32 width = 800, i32 height = 600, bool vsync = false) : m_Title(title), m_Width(width), m_Height(height), m_VSync(vsync) {}

	void Init();

	i32 GetWidth();
	i32 GetHeight();
	void SetVSync(bool state);
	bool VSyncEnabled();
};

