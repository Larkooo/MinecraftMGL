#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <Definitions.h>

class FrameBuffer
{
	u32 m_Id;
	u32 m_TextureId;
	u32 m_RenderBufferId;

	u16 m_Width, m_Height;

public:
	FrameBuffer(u16 width, u16 height);
	~FrameBuffer();

	void Bind();
	void Unbind();
};

