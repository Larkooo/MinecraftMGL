#pragma once

#include <glm/glm.hpp>

#include <Graphics/VertexArray.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/IndexBuffer.h>
//#include <Graphics/Texture.h>
#include <Graphics/Shader.h>
#include <Definitions.h>

#include <list>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;
};

class Mesh
{
	std::vector<Vertex> m_Vertices;
	std::vector<u32> m_Indices;
	//std::list<Texture> m_Textures;

	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_IBO;

public:
	Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<u32> indices);

	static Mesh Cube();

	VertexArray& GetVAO() { return m_VAO; }
	std::vector<Vertex>& GetVertices() { return m_Vertices; }
	std::vector<u32>& GetIndices() { return m_Indices; }
	/*std::list<texture> GetTextures() { return m_textures; }*/

	void Render(Shader& shader);
};

