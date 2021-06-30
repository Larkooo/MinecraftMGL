#include "Graphics/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}


