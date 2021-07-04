#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Definitions.h>

class VertexBuffer
{
	u32 m_Id;

public:
	VertexBuffer(const void* data, u32 size);
	VertexBuffer();
	~VertexBuffer();

	void Bind();
	void Unbind();
};

