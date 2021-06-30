#pragma once

#include <Includes.h>
#define STB_IMAGE_IMPLEMENTATION    
#include <Utils/stb_image.h>

class Texture
{
	u32 m_Id;
	int m_Width, m_Height;

public:
	Texture(const std::string& path);

	int GetWidth() { return m_Width; };
	int GetHeight() { return m_Height; }

	void Bind();
	void Unbind();
};

