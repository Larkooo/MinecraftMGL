#include "Graphics/VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, u32 size)
{
	glGenBuffers(GL_ARRAY_BUFFER, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &m_Id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
