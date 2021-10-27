#include "Core/World.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"

#include <thread>
#include "glm/gtc/integer.hpp"
#include "glm/gtc/noise.hpp"

World::World(i16 seed) : m_Seed(seed)
{
}

World::~World()
{
	if (m_GenerationThread != nullptr)
		m_GenerationThread->join();
	if (m_MeshingThread != nullptr)
		m_MeshingThread->join();
}

void World::Init()
{
	srand(m_Seed);

	m_Player.SetPosition({ rand() % 1000, (rand() % 30) + 10, rand() % 1000 });

	constexpr i32 startX = -(static_cast<i32>(sDimensions.x) / 2);
	constexpr i32 startY = -(static_cast<i32>(sDimensions.y) / 2);

	for (i32 x = startX; x < static_cast<i32>(sDimensions.x) / 2; x++)
	{
		for (i32 y = startY; y < static_cast<i32>(sDimensions.y) / 2; y++)
		{
			const glm::uvec2 localPos = { x + (static_cast<i32>(sDimensions.x) / 2), y + (static_cast<i32>(sDimensions.y) / 2) };
			const glm::ivec2 worldPos = {
				m_Player.GetPosition().x + (x * static_cast<i32>(Chunk::sDimensions.x)),
				m_Player.GetPosition().z + (y * static_cast<i32>(Chunk::sDimensions.z))
			};

			m_Chunks[localPos.x + sDimensions.x * localPos.y] = new Chunk(
				this, 
				localPos,
				worldPos
			);
		}
	}

	Generate();

	// chunk generation thread
	m_GenerationThread = std::make_unique<std::thread>([&]()
	{
		while (Game::Instance()->IsRunning())
		{
			glm::vec2 playerPos = { m_Player.GetPosition().x, m_Player.GetPosition().z };

			/*    
			 *               1
			 *    ---------
			 *    |       |   0.5
			 *    |       |   
			 * 0   ---------
			 *       0.5 
			 */

		//	std::cout << playerPos.x << " " << playerPos.y << std::endl;

			for (Chunk* c : m_Chunks)
			{
				if (c->GetFlag() == Chunk::Flag::MESH_CONSTRUCTING) continue; // avoid getting the threads stepping on each others work

				glm::ivec2 chunkPos = c->GetWorldPosition();

				const glm::ivec2 distance = static_cast<glm::ivec2>(playerPos) - chunkPos;

				bool change = false;
				// x-axis
				if (distance.x > static_cast<i32>(sDimensions.x * Chunk::sDimensions.x) / 2)
				{
					c->SetWorldPosition({ chunkPos.x + static_cast<i32>(sDimensions.x * Chunk::sDimensions.x), chunkPos.y });
					change = true;
				}
				else if (distance.x < -static_cast<i32>(sDimensions.x * Chunk::sDimensions.x) / 2)
				{
					c->SetWorldPosition({ chunkPos.x - static_cast<i32>(sDimensions.x * Chunk::sDimensions.x), chunkPos.y });
					change = true;
				}

				// y-axis
				if (distance.y > static_cast<i32>(sDimensions.y * Chunk::sDimensions.z) / 2)
				{
					c->SetWorldPosition({ chunkPos.x, chunkPos.y + static_cast<i32>(sDimensions.y * Chunk::sDimensions.z) });
					change = true;
				}
				else if (distance.y < -static_cast<i32>(sDimensions.y * Chunk::sDimensions.z) / 2)
				{
					c->SetWorldPosition({ chunkPos.x, chunkPos.y - static_cast<i32>(sDimensions.y * Chunk::sDimensions.z) });
					change = true;
				}

				if (!change)
					continue;

				
				c->Generate();
			}
			//std::this_thread::sleep_for(std::chrono::milliseconds(500)); // check every 500ms
		}
	});


	// mesh constructor thread
	m_MeshingThread = std::make_unique<std::thread>(std::thread([&]
	{
			while (Game::Instance()->IsRunning())
			{
				for (Chunk* c : m_Chunks)
				{
					if (c->GetFlag() == Chunk::Flag::GENERATED)
					{
						/*for (u8 dir = 0; dir < 2; dir++)
						{
							Chunk& front = c->GetNeighbor(dir, false);
							Chunk& back = c->GetNeighbor(dir, true);

							front.GenerateMesh();
							back.GenerateMesh();
						}*/
						c->GenerateMesh();
					}
				}
			}
	}));
}

void World::Generate()
{
	for (Chunk* c : m_Chunks)
	{
		c->Generate();
	}
	for (Chunk* c : m_Chunks)
	{
		c->GenerateMesh();
	}
}

void World::HandleEvents()
{
	m_Player.HandleEvents();
}

void World::Update()
{
	m_Player.Update();
	for (Chunk* c : m_Chunks)
		c->Update();
}

void World::Render()
{
	static Shader shader("./res/shaders/block.vert", "./res/shaders/block.frag");

	shader.Bind();
	
	for (Chunk* c : m_Chunks)
		c->Render(shader);

	shader.Unbind();
}
