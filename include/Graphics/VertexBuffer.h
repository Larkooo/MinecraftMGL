#pragma once

#include <glad/glad.h>
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

