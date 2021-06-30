#pragma once

#include <array>

#include "Block.h"

class Chunk
{
	std::array<Block, 16 * 16 * 16> m_Blocks;

public:
	Chunk() = default;

	// generate all the blocks in the chunk
	void Init();
	// update and render
	void Update();
	void Render();
};

