#pragma once

#include "Definitions.h"

#include <glm/glm.hpp>
#include <unordered_map>

class Block
{
public:
	enum class Type : u8 {
		AIR = 0,
		DIRT = 1,
		WOOD = 2,
		STONE = 3,
		TNT = 4
	};
	
	struct Texture
	{
		glm::vec2 mTop;
		glm::vec2 mSide;
		glm::vec2 mBottom;
	};
	inline static const std::unordered_map<Block::Type, Block::Texture, std::hash<Type>> Map = {
			{ Type::DIRT, { { 2, 0 }, { 2, 0 }, { 2, 0 } } },
			{ Type::WOOD, { { 5, 1 }, { 4, 1 }, { 5, 1 } } },
			{ Type::STONE, { { 1, 0 }, { 1, 0 }, { 1, 0 } } },
			{ Type::TNT, { { 9, 0 }, { 8, 0 }, { 10, 0 } } }
	};

private:
	Type m_Type;

public:
	Block(Type type = Type::DIRT) : m_Type(type) {};

	bool IsSolid() { return m_Type != Type::AIR; }
	Type GetType() { return m_Type; }

	bool operator==(const Block& other)
	{
		return this->m_Type == other.m_Type;
	}
};

