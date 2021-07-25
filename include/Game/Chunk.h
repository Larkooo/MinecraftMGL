#pragma once

#include <array>
#include <thread>

#include "Block.h"
#include "Graphics/Mesh.h"

#include "Definitions.h"

class Chunk
{
public:
	static const u16 sBlocks1D = 16;

private:
	std::array<Block*, sBlocks1D* sBlocks1D* sBlocks1D> m_Blocks;
	//Mesh* m_Mesh = nullptr;

public:
	//std::thread* mThread = nullptr;

public:

	Chunk();

	std::array<Block*, sBlocks1D * sBlocks1D * sBlocks1D>& GetBlocks() { return m_Blocks; }

	//void GenerateMesh();

	// update and render
	void Update();
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

