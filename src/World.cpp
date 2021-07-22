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
		m_Chunks[i] = new Chunk;
		m_Chunks[i]->GenerateMesh();
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
	static Shader shader("./res/shaders/triangle.vert", "./res/shaders/triangle.frag");

	shader.Bind();
	
	for (u32 x = 0; x < sChunks1D; x++)
	{
		for (u32 y = 0; y < sChunks1D; y++)
		{
			for (u32 z = 0; z < sChunks1D; z++)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(x * Chunk::sBlocks1D, y * Chunk::sBlocks1D, z * Chunk::sBlocks1D));

				shader.Set("uModel", model);

				m_Chunks[x + sChunks1D * (y + sChunks1D * z)]->Render(shader);
			}
		}
	}

	shader.Unbind();
}
