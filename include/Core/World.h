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
	static constexpr glm::uvec2 sDimensions = { 4, 4 };

private:
	i16 m_Seed;

	Player m_Player;
	//std::unordered_map<glm::uvec3, Chunk*> m_Chunks;
	std::array<Chunk*, sDimensions.x * sDimensions.y> m_Chunks = { nullptr };

	// instancing thread
	std::unique_ptr<std::thread> m_GenerationThread = nullptr;
	std::unique_ptr<std::thread> m_MeshingThread = nullptr;

public:
	World(i16 seed = INT16_MAX);
	~World();

	Player& GetPlayer() { return m_Player; }

	// generate the chunks
	void Init();
	void Generate();

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
		position.x %= sDimensions.x;
		position.y %= sDimensions.y;

		if (position.x < 0)
			position.x += sDimensions.x;
		if (position.y < 0)
			position.y += sDimensions.y;

		return *m_Chunks[position.x + sDimensions.x * position.y];
	}
};

