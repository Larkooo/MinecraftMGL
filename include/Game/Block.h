#pragma once

enum class BlockType {
	AIR = 0,
	DIRT = 1
};

class Block
{
	BlockType m_Type;
public:
	Block() : m_Type(BlockType::DIRT) {};
};

