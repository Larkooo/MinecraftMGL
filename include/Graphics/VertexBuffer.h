#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Definitions.h>

class VertexBuffer
{
	u32 m_Id;
	u32 m_Size;

public:
	VertexBuffer();
	VertexBuffer(const void* data, u32 size);
	VertexBuffer(const VertexBuffer& other);
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;


	VertexBuffer& operator=(const VertexBuffer& other);
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
};

