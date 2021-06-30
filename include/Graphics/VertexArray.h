#pragma once

#include <Includes.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
	u32 m_Id;

public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();
};

