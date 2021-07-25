#pragma once

#include <glm/glm.hpp>

#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"

#include "Definitions.h"

#include <list>

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
	Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
	Mesh(const Mesh& mesh);

	VertexArray& GetVAO() { return m_VAO; }
	std::vector<Vertex>& GetVertices() { return m_Vertices; }
	std::vector<u32>& GetIndices() { return m_Indices; }

	/*std::list<texture> GetTextures() { return m_textures; }*/

	void Render(Shader& shader);

public:
	static Mesh Cube();
	// TODO : static Mesh Cuboid(u32 width, u32 height, u32 depth);
};

