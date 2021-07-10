#include "Core/World.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"

#include <thread>

void World::Init()
{
	for (size_t i = 0; i < sChunks1D * sChunks1D * sChunks1D; i++)
	{
		m_Chunks[i] = new Chunk(i);
	}
}

void World::HandleEvents()
{
	m_Player.HandleEvents();
}

void World::Update()
{
	m_Player.Update();
}

void World::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	static Mesh mesh = Mesh::Cube();

	static Shader shader("./res/shaders/triangle.vert", "./res/shaders/triangle.frag");

	shader.Bind();

	shader.Set("uProj", Game::Instance()->GetProjection());
	shader.Set("uView", Game::Instance()->GetWorld().GetPlayer().GetCamera().GetView());
	
	std::array<glm::mat4, World::sChunks1D * World::sChunks1D * World::sChunks1D> modelMatrices;

	for (size_t x = 0; x < World::sChunks1D; x++)
	{
		for (size_t y = 0; y < World::sChunks1D; y++)
		{
			for (size_t z = 0; z < World::sChunks1D; z++)
			{
				modelMatrices[x + World::sChunks1D * (y + World::sChunks1D * z)] = glm::scale(
					glm::translate(glm::mat4(1.0f), { x * Chunk::sBlocks1D, y * Chunk::sBlocks1D, z * Chunk::sBlocks1D }), // translate it to the location of the chunk in the 3d dimensional flattened array
					{ Chunk::sBlocks1D, Chunk::sBlocks1D, Chunk::sBlocks1D } // scale it by the number of blocks a chunk has in each dimension
				);
			}
		}
	}

	mesh.GetVAO().Bind();

	VertexBuffer vbo(modelMatrices.data(), sizeof(modelMatrices));
	vbo.Bind();

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)sizeof(glm::vec4));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	vbo.Unbind();

	glDrawElementsInstanced(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, mesh.GetIndices().data(), modelMatrices.size());

	shader.Unbind();
}
