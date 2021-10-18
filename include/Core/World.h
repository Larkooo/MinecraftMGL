#pragma once

#include <array>
#include <map>

#include <Game/Chunk.h>
#include <Game/Player.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

class World
{
public:
	// Number of chunks per dimension
	static constexpr glm::uvec2 sDimensions = { 10, 10 };

private:
	i16 m_Seed;

	Player m_Player;
	//std::unordered_map<glm::uvec3, Chunk*> m_Chunks;
	std::array<Chunk*, sDimensions.x * sDimensions.y> m_Chunks = { nullptr };

	// instancing thread
	std::unique_ptr<std::thread> m_GenerationThread = nullptr;
	// std::unique_ptr<std::thread> m_InstantiationThread = nullptr;

public:
	World(i16 seed = INT16_MAX);
	~World();

	Player& GetPlayer() { return m_Player; }

	// generate the chunks
	void Init();
	void Generate();

	// world coordinate system
	Chunk& GetWorldChunk(glm::ivec2 position)
	{
		glm::ivec2 relativeToPlayer = { m_Player.GetPosition().x - position.x, m_Player.GetPosition().y - position.y };
	}

	// update and render
	void HandleEvents();
	void Update();
	void Render();

	/*Chunk& operator[](size_t index) const
	{
		return *m_Chunks[index];
	}*/
	// Local coordinate system (around player)
	Chunk& operator[](glm::ivec2 position) const
	{
		// wrapping around using modulo
		//position.x %= sDimensions.x;
		//position.y %= sDimensions.y;

		return *m_Chunks[position.x + sDimensions.x * position.y];
	}
};

