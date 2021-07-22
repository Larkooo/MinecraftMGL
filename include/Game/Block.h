#pragma once

#include "Definitions.h"

class Block
{
public:
	enum class Type {
		AIR = 0,
		DIRT = 1,
		STONE = 2,
		OBSIDIAN = 3
	};

private:
	Type m_Type;

public:
	Block(Type type = Type::DIRT) : m_Type(type) {};

	bool IsSolid();
	Type GetType() { return m_Type; }

	bool operator==(const Block& other)
	{
		return this->m_Type == other.m_Type;
	}
};

