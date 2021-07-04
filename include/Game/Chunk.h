#pragma once

#include <array>
#include <thread>

#include "Block.h"

#include "Definitions.h"

class Chunk
{
public:
	static const u32 sBlocks = 16 * 16 * 16;

private:
	std::array<Block, sBlocks> m_Blocks;

public:
	std::thread* mThread = nullptr;

public:

	Chunk() = default;

	std::array<Block, 16 * 16 * 16>& GetBlocks() { return m_Blocks; }

	// generate all the blocks in the chunk
	void Init();
	// update and render
	void Update();
	void Render();
};

