#pragma once

#include <array>
#include <thread>

#include "Block.h"

#include "Graphics/Shader.h"

class MeshConstructor;
class Mesh;
class World;

class Chunk
{
public:
	static constexpr glm::uvec3 sDimensions = { 32, 128, 32 };

	typedef std::array<Block, sDimensions.x* sDimensions.y* sDimensions.z> Blocks;
	typedef std::pair<glm::mat4, glm::mat3x2> BlockInstance;

	enum class Flag
	{
		IDLE,
		GENERATING,
		GENERATED,
		MESH_CONSTRUCTED,
		MESH_CREATED
	};

	Flag mFlag = Flag::IDLE;
private:
	glm::uvec2 m_LocalPosition;
	glm::ivec2 m_WorldPosition;

	Blocks m_Blocks;
	World* m_World = nullptr;

	// a mesh constructor, to construct the mesh on a another thread
	MeshConstructor* m_MeshConstructor = nullptr;
	// the mesh, that we create on the main thread
	std::unique_ptr<Mesh> m_Mesh = nullptr;

	// mat4 is the model matrix and mat3x2 is the three tiles texture coordinates (top, side, bottom)
	std::vector<std::pair<glm::mat4, glm::mat3x2>> m_InstancedBlocks;

public:

	Chunk(World* world, glm::uvec2 localPos, glm::vec2 worldPos);
	Chunk() = delete;

	glm::uvec2 GetLocalPosition() const { return m_LocalPosition; }
	glm::ivec2 GetWorldPosition() const { return m_WorldPosition; }

	Blocks& GetBlocks() { return m_Blocks; }
	World& GetWorld() { return *m_World; }

	void SetWorldPosition(glm::ivec2 position)
	{
		m_WorldPosition = position;
	};

	void Generate();

	// update and render
	void Update();
	void GenerateMesh();
	void InstantiateBlocks();
	void Render(Shader& shader);

	Block& operator[](size_t index)
	{
		return m_Blocks[index];
	}
	Block& operator[](glm::uvec3 position)
	{
		return m_Blocks[position.x + sDimensions.x * (position.y + sDimensions.y * position.z)];
	}
};