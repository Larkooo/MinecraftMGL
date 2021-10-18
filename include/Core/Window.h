#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Definitions.h>

class Window
{
	std::string m_Title;
	i32 m_Width;
	i32 m_Height;
	bool m_VSync;

	GLFWwindow* m_Window = nullptr;

public:
	Window(const std::string& title = "Window", i32 width = 800, i32 height = 600, bool vsync = true) : m_Title(title), m_Width(width), m_Height(height), m_VSync(vsync) {}
	Window(const Window& other);
	Window(Window&& other) noexcept;
	~Window();

	void Init();

	i32 GetWidth() const;
	i32 GetHeight() const;
	GLFWwindow* GetGLFWWindow() const { return m_Window; }

	void SetWidth(i32 width) { m_Width = width; }
	void SetHeight(i32 height) { m_Height = height; }

	void SetVSync(bool state);
	bool VSyncEnabled() const;

	Window& operator=(const Window& other);
	Window& operator=(Window&& other) noexcept;
};

