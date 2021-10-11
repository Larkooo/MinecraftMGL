#include "Core/World.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"

#include <thread>
#include "glm/gtc/noise.hpp"

World::World(i16 seed) : m_Seed(seed)
{
}

World::~World()
{
	//m_InstantiationThread->join();
	//m_GenerationThread->join();
}

void World::Init()
{
	srand(m_Seed);

	// generate
	for (u32 x = 0; x < sDimensions.x; x++)
	{
		for (u32 y = 0; y < sDimensions.y; y++)
		{
			for (u32 z = 0; z < sDimensions.z; z++)
			{
				m_Chunks[x + sDimensions.x * (y + sDimensions.y * z)] = new Chunk(this, {x, y, z });
				//m_Chunks[x + sChunks1D * (y + sChunks1D * z)]->GenerateBlocks();
			}
		}
	}

	// x z = chunks
	// x2 z2 = blocks
	// y y2 = vertical axis chunks and blocks
	for (u32 x = 0; x < sDimensions.x; x++)
	{
		for (u32 z = 0; z < sDimensions.z; z++)
		{

			// loop through horizontal blocks
			for (u32 x2 = 0; x2 < Chunk::sBlocks1D; x2++)
			{
				for (u32 z2 = 0; z2 < Chunk::sBlocks1D; z2++)
				{
					constexpr u32 maxHeight = (Chunk::sBlocks1D * sDimensions.y) / 1.5f;
					// between 0 and 1 (+ 1 / 2)
					const float noise = (glm::simplex(glm::vec2(m_Seed + (x * Chunk::sBlocks1D) + x2, m_Seed + (z * Chunk::sBlocks1D) + z2) * 0.03f) + 1) / 2;
					//printf("%f\n", noise);
					const u32 height = static_cast<u32>(noise * maxHeight);


					// loop through vertical blocks
					for (u8 y = 0; y < sDimensions.y; y++)
					{
						for (u8 y2 = 0; y2 < Chunk::sBlocks1D; y2++)
						{
							if ((y * Chunk::sBlocks1D) + y2 < height)
								if ((y * Chunk::sBlocks1D) + y2 < noise * (maxHeight / 2))
									(*this)[{ x, y, z }][{ x2, y2, z2 }] = Block(Block::Type::STONE);
								else
									(*this)[{ x, y, z }][{ x2, y2, z2 }] = Block(Block::Type::DIRT);
								
						}
					}
				}
			}
		}
	}

	m_Player.SetPosition({ (sDimensions.x / 2) * Chunk::sBlocks1D, sDimensions.y * Chunk::sBlocks1D, (sDimensions.x / 2) * Chunk::sBlocks1D });

	//m_GenerationThread = std::make_unique<std::thread>(std::thread([&] {for (Chunk* chunk : m_Chunks) { chunk->GenerateBlocks(); chunk->InstantiateBlocks(); } }));
	// block instancing thread
	/*const auto bf = std::chrono::high_resolution_clock::now();
	m_InstantiationThread = std::make_unique<std::thread>(std::thread([&] { }));
	m_InstantiationThread->join();
	const auto af = std::chrono::high_resolution_clock::now();*/

	for (Chunk* chunk : m_Chunks) chunk->InstantiateBlocks();

	//std::cout << std::chrono::duration_cast<std::chrono::microseconds>(af - bf).count() / 1000.0f << std::endl;
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
	static Shader shader("./res/shaders/block.vert", "./res/shaders/block.frag");

	shader.Bind();
	
	for (Chunk* chunk : m_Chunks)
		//if (chunk->GetPosition())
		chunk->Render(shader);

	shader.Unbind();
}
