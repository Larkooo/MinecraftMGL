#pragma once

#include "Definitions.h"

enum class BlockType {
	AIR = 0,
	DIRT = 1
};

class Block
{
	u32 m_Id;
	BlockType m_Type;

public:
	Block(u32 id) : m_Id(id), m_Type(BlockType::DIRT) {};
	Block() = delete;

	u32 GetId() { return m_Id; }
	BlockType GetType() { return m_Type; }
};

