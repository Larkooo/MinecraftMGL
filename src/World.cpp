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
	//m_InstantiationThread->join();
	if (m_GenerationThread != nullptr)
		m_GenerationThread->join();
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
			const glm::uvec2 localPos = { x + static_cast<i32>(sDimensions.x) / 2, y + static_cast<i32>(sDimensions.y) / 2 };
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

				u32 dist = 200;
				// 0 0.5
				if (glm::distance(playerPos.x, (float)(*this)[{ 0, sDimensions.y / 2 }].GetWorldPosition().x) > dist)
				{
					for (u32 y = 0; y < sDimensions.y; y++)
					{
						Chunk& chunk = (*this)[{ 0, y }];
						chunk.SetWorldPosition({ playerPos.x + glm::distance(playerPos.x, (float)(*this)[{ 0, sDimensions.y / 2 }].GetWorldPosition().x) * 2, chunk.GetWorldPosition().y });
						chunk.Generate();
						chunk.InstantiateBlocks();
						//chunk.InstantiateBlocks();
					}
				}
				// 1 0.5
				else if (glm::distance(playerPos.x, (float)(*this)[{ sDimensions.x - 1, sDimensions.y / 2 }].GetWorldPosition().x) > dist)
				{
					for (u32 y = 0; y < sDimensions.y; y++)
					{
						Chunk& chunk = (*this)[{ sDimensions.x - 1, y }];
						chunk.SetWorldPosition({ playerPos.x - glm::distance(playerPos.x, (float)(*this)[{ 0, sDimensions.y / 2 }].GetWorldPosition().x) * 2, chunk.GetWorldPosition().y });
						chunk.Generate();
						chunk.InstantiateBlocks();
						//chunk.InstantiateBlocks();
					}
				}
				// 0.5 0
				else if (glm::distance(playerPos.y, (float)(*this)[{ sDimensions.y / 2, 0 }].GetWorldPosition().y) > dist)
				{
					for (u32 x = 0; x < sDimensions.x; x++)
					{
						Chunk& chunk = (*this)[{ x, 0 }];
						chunk.SetWorldPosition({ chunk.GetWorldPosition().x, playerPos.y + glm::distance(playerPos.y, (float)(*this)[{ sDimensions.y / 2, 0 }].GetWorldPosition().y) * 2 });
						chunk.Generate();
						chunk.InstantiateBlocks();
						//chunk.InstantiateBlocks();
					}
				}
				// 0.5 1
				else if (glm::distance(playerPos.y, (float)(*this)[{ sDimensions.y / 2, sDimensions.y - 1 }].GetWorldPosition().y) > dist)
				{
					for (u32 x = 0; x < sDimensions.x; x++)
					{
						Chunk& chunk = (*this)[{ x, sDimensions.y - 1 }];
						chunk.SetWorldPosition({ chunk.GetWorldPosition().x, playerPos.y - glm::distance(playerPos.y, (float)(*this)[{ sDimensions.y / 2, sDimensions.y - 1 }].GetWorldPosition().y) * 2 });
						chunk.Generate();
						chunk.InstantiateBlocks();
						//chunk.InstantiateBlocks();
					}
				}



				/*for (Chunk* c : m_Chunks)
				{
					const i32 distance = glm::distance(glm::ivec2{ m_Player.GetPosition().x, m_Player.GetPosition().z }, c->GetWorldPosition());
					if (distance > sqrtf(powf(sDimensions.x * Chunk::sDimensions.x, 2) + powf(sDimensions.y * Chunk::sDimensions.z, 2)))
					{
						c->SetWorldPosition();
						c->Generate();
					}
				}*/
				std::this_thread::sleep_for(std::chrono::milliseconds(500)); // check every 500ms
			}
	});
	/*m_GenerationThread = std::make_unique<std::thread>([&]
	{
		while (Game::Instance()->IsRunning())
			for (Chunk* c : m_Chunks)
			{
				const glm::vec3 distance = m_Player.GetPosition() - c->GetWorldPosition();
				if (distance.x > 100)
				{
					
				}
				else if (distance.z > 100)
				{

				}
			}

	});*/

	/*m_GenerationThread = std::make_unique<std::thread>([&]
		{
			while (Game::Instance()->IsRunning())
				if (glm::distance(m_Player.GetPosition(), m_Chunks[0]->GetWorldPosition()) > sDimensions.x * Chunk::sBlocks1D)
					Generate();

		});*/

	//m_GenerationThread = std::make_unique<std::thread>(std::thread([&] {for (Chunk* chunk : m_Chunks) { chunk->GenerateBlocks(); chunk->InstantiateBlocks(); } }));
	// block instancing thread
	/*const auto bf = std::chrono::high_resolution_clock::now();
	m_InstantiationThread = std::make_unique<std::thread>(std::thread([&] { }));
	m_InstantiationThread->join();
	const auto af = std::chrono::high_resolution_clock::now();*/

	//std::cout << std::chrono::duration_cast<std::chrono::microseconds>(af - bf).count() / 1000.0f << std::endl;
}

void World::Generate()
{
	for (Chunk* c : m_Chunks)
	{
		c->Generate();
	}
	for (Chunk* c : m_Chunks)
	{
		c->InstantiateBlocks();
	}
	//glm::vec3 playerPos = m_Player.GetPosition();

	//// generate
	//constexpr i32 startX = -static_cast<i32>(sDimensions.x) / 2;
	//constexpr i32 startY = -static_cast<i32>(sDimensions.y) / 2;
	//constexpr i32 startZ = -static_cast<i32>(sDimensions.z) / 2;

	//for (int x = startX; x < static_cast<i32>(sDimensions.x / 2); x++)
	//{
	//	for (int y = startY; y < static_cast<i32>(sDimensions.y / 2); y++)
	//	{
	//		for (int z = startZ; z < static_cast<i32>(sDimensions.z / 2); z++)
	//		{
	//			m_Chunks[(x + (sDimensions.x / 2)) + sDimensions.x * ((y + (sDimensions.y / 2)) + sDimensions.y * (z + (sDimensions.z / 2)))] = new Chunk(
	//				this, 
	//				{ x + (sDimensions.x / 2), y + (sDimensions.y / 2), z + (sDimensions.z / 2) }, 
	//				static_cast<glm::ivec3>(m_Player.GetPosition()) + (glm::ivec3{ x, y, z } * static_cast<i32>(Chunk::sBlocks1D)));
	//			//m_Chunks[x + sChunks1D * (y + sChunks1D * z)]->GenerateBlocks();
	//		}
	//	}
	//}

	//// x z = chunks
	//// x2 z2 = blocks
	//// y y2 = vertical axis chunks and blocks
	//for (u32 x = 0; x < sDimensions.x; x++)
	//{
	//	for (u32 z = 0; z < sDimensions.z; z++)
	//	{

	//		// loop through horizontal blocks
	//		for (u32 x2 = 0; x2 < Chunk::sBlocks1D; x2++)
	//		{
	//			for (u32 z2 = 0; z2 < Chunk::sBlocks1D; z2++)
	//			{
	//				const u32 maxHeight = m_Player.GetPosition().y;
	//				// between 0 and 1 (+ 1 / 2)
	//				const float noise = (glm::simplex(glm::vec2(m_Seed + (x * Chunk::sBlocks1D) + x2, m_Seed + (z * Chunk::sBlocks1D) + z2) * 0.03f) + 1) / 2;
	//				//printf("%f\n", noise);
	//				const u32 height = static_cast<u32>(noise * maxHeight);


	//				// loop through vertical blocks
	//				for (u8 y = 0; y < sDimensions.y; y++)
	//				{
	//					for (u8 y2 = 0; y2 < Chunk::sBlocks1D; y2++)
	//					{
	//						if ((y * Chunk::sBlocks1D) + y2 < height)
	//							if ((y * Chunk::sBlocks1D) + y2 < noise * (maxHeight / 2))
	//								(*(*this)[{ x, y, z }])[{ x2, y2, z2 }] = Block(Block::Type::STONE);
	//							else
	//								(*(*this)[{ x, y, z }])[{ x2, y2, z2 }] = Block(Block::Type::DIRT);

	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//for (Chunk* c : m_Chunks)
	//	c->InstantiateBlocks();
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
