#include "Graphics/Mesh.h"

#include "Core/Game.h"
#include "Core/World.h"

Mesh::Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<u32> indices) : m_Vertices(vertices), m_Indices(indices)
{
	m_Vertices = vertices;
	m_Indices = indices;

	m_VAO.Bind();
	
	m_VBO.Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
	//// tile
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tile));
	//// factor
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, factor));

	m_VBO.Unbind();
	m_VAO.Unbind();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
{
	m_Vertices = vertices;
	m_Indices = indices;

	m_VAO.Bind();

	m_VBO.Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
	//// tile
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tile));
	//// factor
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, factor));

	m_VBO.Unbind();
	m_VAO.Unbind();
}

Mesh::Mesh(const Mesh& mesh)
{
	m_Vertices = mesh.m_Vertices;
	m_Indices = mesh.m_Indices;

	m_VAO = mesh.m_VAO;
	m_VBO = mesh.m_VBO;
	m_IBO = mesh.m_IBO;
}

Mesh Mesh::Cube()
{

    std::initializer_list<Vertex> vertices = {
		 // Back
		{ {-0.5f,  0.5f, -0.5f},	{0.f, 0.f, -1.f},	{1.f, 0.f} },
		{ { 0.5f,  0.5f, -0.5f},	{0.f, 0.f, -1.f},	{0.f, 0.f} },
		{ {-0.5f, -0.5f, -0.5f},	{0.f, 0.f, -1.f},	{1.f, 1.f} },
		{ { 0.5f,  0.5f, -0.5f},	{0.f, 0.f, -1.f},	{0.f, 0.f} },
		{ { 0.5f, -0.5f, -0.5f},	{0.f, 0.f, -1.f},	{0.f, 1.f} },
		{ {-0.5f, -0.5f, -0.5f},	{0.f, 0.f, -1.f},	{1.f, 1.f} },

		// Front
		{ { 0.5f,  0.5f,  0.5f},	{0.f, 0.f, 1.f},	{1.f, 0.f} },
		{ {-0.5f,  0.5f,  0.5f},	{0.f, 0.f, 1.f},	{0.f, 0.f} },
		{ {-0.5f, -0.5f,  0.5f},	{0.f, 0.f, 1.f},	{0.f, 1.f} },
		{ { 0.5f, -0.5f,  0.5f},	{0.f, 0.f, 1.f},	{1.f, 1.f} },
		{ { 0.5f,  0.5f,  0.5f},	{0.f, 0.f, 1.f},	{1.f, 0.f} },
		{ {-0.5f, -0.5f,  0.5f},	{0.f, 0.f, 1.f},	{0.f, 1.f} },

		// Left
		{ {-0.5f,  0.5f,  0.5f},	{-1.f, 0.f, 0.f},	{1.f, 0.f} },
		{ {-0.5f,  0.5f, -0.5f},	{-1.f, 0.f, 0.f},	{0.f, 0.f} },
		{ {-0.5f, -0.5f,  0.5f},	{-1.f, 0.f, 0.f},	{1.f, 1.f} },
		{ {-0.5f,  0.5f, -0.5f},	{-1.f, 0.f, 0.f},	{0.f, 0.f} },
		{ {-0.5f, -0.5f, -0.5f},	{-1.f, 0.f, 0.f},	{0.f, 1.f} },
		{ {-0.5f, -0.5f,  0.5f},	{-1.f, 0.f, 0.f},	{1.f, 1.f} },
		  
		// Right
		{ { 0.5f,  0.5f, -0.5f},	{1.f, 0.f, 0.f},	{1.f, 0.f} },
		{ { 0.5f,  0.5f,  0.5f},	{1.f, 0.f, 0.f},	{0.f, 0.f} },
		{ { 0.5f, -0.5f,  0.5f},	{1.f, 0.f, 0.f},	{0.f, 1.f} },
		{ { 0.5f, -0.5f, -0.5f},	{1.f, 0.f, 0.f},	{1.f, 1.f} },
		{ { 0.5f,  0.5f, -0.5f},	{1.f, 0.f, 0.f},	{1.f, 0.f} },
		{ { 0.5f, -0.5f,  0.5f},	{1.f, 0.f, 0.f},	{0.f, 1.f} },
		  
		// Top 
		{ { 0.5f,  0.5f, -0.5f},	{0.f, 1.f, 0.f},	{1.f, 0.f} },
		{ {-0.5f,  0.5f, -0.5f},	{0.f, 1.f, 0.f},	{0.f, 0.f} },
		{ {-0.5f,  0.5f,  0.5f},	{0.f, 1.f, 0.f},	{0.f, 1.f} },
		{ { 0.5f,  0.5f, -0.5f},	{0.f, 1.f, 0.f},	{1.f, 0.f} },
		{ {-0.5f,  0.5f,  0.5f},	{0.f, 1.f, 0.f},	{0.f, 1.f} },
		{ { 0.5f,  0.5f,  0.5f},	{0.f, 1.f, 0.f},	{1.f, 1.f} },
		  
		// Bottom
		{ { 0.5f, -0.5f, -0.5f},	{0.f, -1.f, 0.f},	{0.f, 0.f} },
		{ {-0.5f, -0.5f,  0.5f},	{0.f, -1.f, 0.f},	{1.f, 1.f} },
		{ {-0.5f, -0.5f, -0.5f},	{0.f, -1.f, 0.f},	{1.f, 0.f} },
		{ { 0.5f, -0.5f, -0.5f},	{0.f, -1.f, 0.f},	{0.f, 0.f} },
		{ { 0.5f, -0.5f,  0.5f},	{0.f, -1.f, 0.f},	{0.f, 1.f} },
		{ {-0.5f, -0.5f,  0.5f},	{0.f, -1.f, 0.f},	{1.f, 1.f} }
	};

    std::initializer_list<u32> indices = {
        0, 1, 2, 3, 4, 5, 
		6, 7, 8, 9, 10, 11, 
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 
		30, 31, 32, 33, 34, 35
    };
        
    return Mesh(vertices, indices);
}

void Mesh::Render(Shader& shader)
{	
    shader.Bind();

    shader.Set("uProj", Game::Instance()->GetProjection());
    shader.Set("uView", Game::Instance()->GetWorld().GetPlayer().GetCamera().GetView());

	m_VAO.Bind();
	glDrawElements(GL_TRIANGLES, (GLsizei) m_Vertices.size(), GL_UNSIGNED_INT, m_Indices.data());
}
