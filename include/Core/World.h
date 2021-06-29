#pragma once

#include <array>

#include <Game/Chunk.h>

class World
{
	std::array<std::unique_ptr<Chunk>, 24> m_Chunks;

public:
	World() = default;

	// generate the chunks
	void Init();
	// update and render
	void Update();
	void Render();
};

