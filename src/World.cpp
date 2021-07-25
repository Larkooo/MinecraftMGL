#include "Core/World.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"

#include <thread>

World::World(i64 seed) : m_Seed(seed)
{
}

World::~World()
{
	m_InstantiationThread->join();
}

void World::Init()
{
	srand(m_Seed);

	for (u32 x = 0; x < sChunks1D; x++)
	{
		for (u32 y = 0; y < sChunks1D; y++)
		{
			for (u32 z = 0; z < sChunks1D; z++)
			{
				m_Chunks[x + sChunks1D * (y + sChunks1D * z)] = new Chunk(this, { x, y, z });
			}
		}
	}

	// block instancing thread
	m_InstantiationThread = std::make_unique<std::thread>(std::thread([&] {for (Chunk* chunk : m_Chunks) chunk->InstanceBlocks(); }));
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
	
	for (Chunk* chunk : m_Chunks)
		chunk->Render(shader);

	shader.Unbind();
}
