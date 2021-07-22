#pragma once

#include <array>

#include <Game/Chunk.h>
#include <Game/Player.h>

class World
{
public:
	// Number of chunks per dimension
	static const u32 sChunks1D = 8;

private:
	Player m_Player;
	std::array<Chunk*, sChunks1D * sChunks1D * sChunks1D> m_Chunks;

public:
	World() = default;

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
};

