#include "Graphics/VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, u32 size) : m_Size(size)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
{
	m_Size = other.m_Size;

	glGenBuffers(1, &m_Id);

	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_Size);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
	m_Id = other.m_Id;
	m_Size = other.m_Size;

	other.m_Id = NULL;
}

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &m_Id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
{
	if (this == &other)
		return *this;

	m_Size = other.m_Size;

	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_Size);

	return *this;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this == &other)
		return *this;

	this->~VertexBuffer(); // delete old buffer

	m_Id = other.m_Id;
	m_Size = other.m_Size;

	other.m_Id = NULL;

	return *this;
}
