#pragma once

#include <array>
#include <thread>

#include "Block.h"

#include "Definitions.h"

class Chunk
{
public:
	static const u32 sBlocks1D = 16;

private:
	u32 m_Id;
	std::array<Block*, sBlocks1D * sBlocks1D * sBlocks1D> m_Blocks;

public:
	std::thread* mThread = nullptr;

public:

	Chunk(u32 id) : m_Id(id) 
	{
		for (size_t i = 0; i < sBlocks1D * sBlocks1D * sBlocks1D; i++)
		{
			m_Blocks[i] = new Block(i);
		}
	};

	std::array<Block*, 16 * 16 * 16>& GetBlocks() { return m_Blocks; }

	// update and render
	void Update();
	void Render();
};

