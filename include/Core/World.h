#pragma once

#include <array>

#include <Game/Chunk.h>
#include <Game/Player.h>

class World
{
public:
	// Number of chunks per dimension
	static const u32 sChunks1D = 4;

private:
	i64 m_Seed;

	Player m_Player;
	std::array<Chunk*, sChunks1D* sChunks1D* sChunks1D> m_Chunks = { nullptr };

	// instancing thread
	std::unique_ptr<std::thread> m_GenerationThread = nullptr;
	std::unique_ptr<std::thread> m_InstantiationThread = nullptr;

public:
	World(i64 seed = 0);
	~World();

	Player& GetPlayer() { return m_Player; }

	// generate the chunks
	void Init();
	// update and render
	void HandleEvents();
	void Update();
	void Render();

	Chunk& operator[](size_t index)
	{
		return *m_Chunks[index];
	}
	Chunk& operator[](glm::vec3 position)
	{
		return *m_Chunks[position.x + sChunks1D * (position.y + sChunks1D * position.z)];
	}
};

