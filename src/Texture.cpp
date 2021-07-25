#include "Graphics/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <Utils/stb_image.h>

Texture::Texture(const std::string& path)
{
	//stbi_set_flip_vertically_on_load(true);

	int nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &nrChannels, 0);

	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data)
	{
		//m_Path = filePath;

		// by default on rgb
		GLenum format = GL_RGB;

		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
