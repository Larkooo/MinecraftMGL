#pragma once

#include <array>
#include <thread>

#include "Block.h"
#include "Graphics/Mesh.h"

#include "Definitions.h"

class World;

class Chunk
{
public:
	static const u16 sBlocks1D = 16;

private:
	glm::vec3 m_Position;
	std::array<Block*, sBlocks1D* sBlocks1D* sBlocks1D> m_Blocks;
	World* m_World = nullptr;

	// mat4 is the model matrix and mat3x2 is the three tiles texture coordinates (top, side, bottom)
	std::vector<std::pair<glm::mat4, glm::mat3x2>> m_InstancedBlocks;

public:
	//std::thread* mThread = nullptr;

public:

	Chunk(World* world, glm::vec3 pos);
	Chunk() = delete;

	glm::vec3 GetPosition() { return m_Position; }
	std::array<Block*, sBlocks1D * sBlocks1D * sBlocks1D>& GetBlocks() { return m_Blocks; }
	World& GetWorld() { return *m_World; }

	//void GenerateMesh();

	// update and render
	void Update();
	void InstanceBlocks();
	void Render(Shader& shader);

	Block& operator[](size_t index)
	{
		return *m_Blocks[index];
	}
	Block& operator[](glm::vec3 position)
	{
		return *m_Blocks[position.x + sBlocks1D * (position.y + sBlocks1D * position.z)];
	}
};