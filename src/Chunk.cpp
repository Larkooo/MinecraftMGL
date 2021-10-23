#include "Game/Chunk.h"

#include "Graphics/MeshData.h"
#include "Core/Game.h"
#include "Core/World.h"

#include <functional>
#include <array>
#include <mutex>

#include <glm/gtc/noise.hpp>

#include "Graphics/MeshConstructor.h"

Chunk::Chunk(World* world, glm::uvec2 localPos, glm::vec2 worldPos) : m_LocalPosition(localPos), m_WorldPosition(worldPos), m_World(world)
{
	m_VBO = std::make_unique<VertexBuffer>();
	/*u32 height = rand() % sDimensions.y;
	for (u32 x = 0; x < sDimensions.x; x++)
	{
		for (u32 y = 0; y < sDimensions.y; y++)
		{
			for (u32 z = 0; z < sDimensions.z; z++)
			{
				if (y < height)
					(*this)[{ x, y, z }] = Block(Block::Type::WOOD);
				else
					(*this)[{ x, y, z }] = Block();
			}
		}
	}*/

	/*for (u32 x = 0; x < sDimensions.x; x++)
	{
		for (u32 z = 0; z < sDimensions.z; z++)
		{
			const float noise = (glm::simplex(glm::vec2{x + m_WorldPosition.x, z + m_WorldPosition.y } * 0.002f) + 1) / 2.0f;

			for (u32 y = 0; y < sDimensions.y; y++)
			{
				if (y < noise * sDimensions.y)
					(*this)[{ x, y, z }] = Block(Block::Type::WOOD);
				else
					(*this)[{ x, y, z }] = Block();
			}
		}
	}*/

	for (u32 i = 0; i < sDimensions.x * sDimensions.y * sDimensions.z; i++)
	{
		m_Blocks[i] = Block();
	}
}

void Chunk::InstantiateBlocks()
{
	//std::mutex mtx;

	//mtx.lock();
	m_InstancedBlocks.clear();

	auto airBlockAround = [&](glm::vec3 position)
	{
		// 0 = x, 1 = y, 2 = z
		for (u8 axis = 0; axis < 3; axis++)
		{
			glm::ivec3 frontBlock = position, backBlock = position;
			frontBlock[axis]++; backBlock[axis]--;

			if ((frontBlock.y < 0 || frontBlock.y > sDimensions.y - 1) || (backBlock.y < 0 || backBlock.y > sDimensions.y - 1))
				return true;

			// 0 = x, 1 = z
			// ReSharper disable once IdentifierTypo
			for (u8 saxis = 0; saxis < 2; saxis++)
			{
				glm::ivec2 otherChunk;
				glm::ivec3 otherBlock;

				glm::u8 blockAxis = saxis;
				if (saxis == 1)
					blockAxis++;

				if (frontBlock[blockAxis] < 0)
				{
					otherChunk = m_LocalPosition; otherChunk[saxis]--;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = frontBlock; otherBlock[blockAxis] = sDimensions[blockAxis] - 1;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[frontBlock].IsSolid())
						return true;
					goto skip;
				}
				if (frontBlock[blockAxis] > sDimensions[blockAxis] - 1)
				{
					otherChunk = m_LocalPosition; otherChunk[saxis]++;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = frontBlock; otherBlock[blockAxis] = 0;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[backBlock].IsSolid())
						return true;
					goto skip;
				}

				if (backBlock[blockAxis] < 0)
				{
					otherChunk = m_LocalPosition; otherChunk[saxis]--;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = backBlock; otherBlock[blockAxis] = sDimensions[blockAxis] - 1;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[frontBlock].IsSolid())
						return true;
					goto skip;
				}
				if (backBlock[blockAxis] > sDimensions[blockAxis] - 1)
				{
					otherChunk = m_LocalPosition; otherChunk[saxis]++;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = backBlock; otherBlock[blockAxis] = 0;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[backBlock].IsSolid())
						return true;
					goto skip;
				}
			}

			if (!(*this)[frontBlock].IsSolid() || !(*this)[backBlock].IsSolid())
				return true;

		skip:
			{}
		}

		return false;
	};

	//auto airBlockAround = [&](glm::uvec3 position)
	//{
	//	// check neighbor chunks
	//	{
	//		if (m_Position.x > 0 && m_Position.x < World::sChunks1D - 1)
	//			switch (position.x)
	//			{
	//			case 0:
	//			{
	//				//std::cout << " haha" << std::endl;
	//				Chunk otherChunk = (*m_World)[{ m_Position.x - 1, m_Position.y, m_Position.z }];
	//				//std::cout << (m_Position.x * sBlocks1D) + position.x << " " << (m_Position.y * sBlocks1D) + position.y << " " << (m_Position.z * sBlocks1D) + position.z << std::endl;
	//				//std::cout << (otherChunk.GetPosition().x * sBlocks1D) + position.x << " " << (otherChunk.GetPosition().y * sBlocks1D) + position.y << " " << (otherChunk.GetPosition().z * sBlocks1D) + position.z << std::endl;
	//				if (!otherChunk[{ sBlocks1D - 1, position.y, position.z }].IsSolid())
	//				{
	//					//std::cout << " haha" << std::endl;
	//					return true;
	//				}
	//					
	//				break;
	//			}
	//			case sBlocks1D - 1:
	//			{
	//				Chunk otherChunk = (*m_World)[{ m_Position.x + 1, m_Position.y, m_Position.z }];
	//				if (!otherChunk[{ 0, position.y, position.z }].IsSolid())
	//					return true;
	//				break;
	//			}
	//			}
	//		if (m_Position.y > 0 && m_Position.y < World::sChunks1D - 1)
	//			switch (position.y)
	//			{
	//			case 0:
	//			{
	//				Chunk otherChunk = (*m_World)[{ m_Position.x, m_Position.y - 1, m_Position.z }];
	//				if (!otherChunk[{ position.x, sBlocks1D - 1, position.z }].IsSolid())
	//					return true;
	//				break;
	//			}
	//			case sBlocks1D - 1:
	//			{
	//				Chunk otherChunk = (*m_World)[{ m_Position.x, m_Position.y + 1, m_Position.z }];
	//				if (!otherChunk[{ position.x, 0, position.z }].IsSolid())
	//					return true;
	//				break;
	//			}
	//			}
	//		if (m_Position.z > 0 && m_Position.z < World::sChunks1D - 1)
	//			switch (position.z)
	//			{
	//			case 0:
	//			{
	//				Chunk otherChunk = (*m_World)[{ m_Position.x, m_Position.y, m_Position.z + 1 }];
	//				if (!otherChunk[{ position.x, position.y, sBlocks1D - 1 }].IsSolid())
	//					return true;
	//				break;
	//			}
	//			case sBlocks1D - 1:
	//			{
	//				Chunk otherChunk = (*m_World)[{ m_Position.x, m_Position.y, m_Position.z - 1 }];
	//				if (!otherChunk[{ position.x, position.y, 0 }].IsSolid())
	//					return true;
	//				break;
	//			}
	//			}
	//	}

	//	for (i8 x = -1; x <= 1; x++)
	//	{
	//		for (i8 y = -1; y <= 1; y++)
	//		{
	//			for (i8 z = -1; z <= 1; z++)
	//			{
	//				glm::uvec3 pos = { position.x + x, position.y + y, position.z + z };

	//				if (pos.x < 0)
	//				{
	//					Chunk otherChunk = (*m_World)[{ m_Position.x - 1, m_Position.y, m_Position.z }];
	//					if (!otherChunk[])
	//				}

	//				if (!m_Blocks[pos.x + sBlocks1D * (pos.y + sBlocks1D * pos.z)]->IsSolid())
	//					return true;
	//			}
	//		}
	//	}
	//	return false;
	//};

	//std::cout << m_WorldPosition.x << " " << m_WorldPosition.y << std::endl;
	for (u32 x = 0; x < sDimensions.x; x++)
	{
		for (u32 y = 0; y < sDimensions.y; y++)
		{
			for (u32 z = 0; z < sDimensions.z; z++)
			{
				if (!airBlockAround({ x, y, z }) || !(*this)[{ x, y, z }].IsSolid())
					continue;

				//printf("haha\n");

				glm::mat4 model(1.0f);
				model = glm::translate(model, { m_WorldPosition.x + static_cast<i32>(x), y, m_WorldPosition.y + static_cast<i32>(z) });
				/*model = glm::translate(model, {
					(m_World->GetPlayer().GetPosition().x + (static_cast<i32>(m_LocalPosition.x) - static_cast<i32>(World::sDimensions.x / 2)) * static_cast<i32>(sDimensions.x)) + i32(x),
					y,
					(m_World->GetPlayer().GetPosition().z + (static_cast<i32>(m_LocalPosition.y) - static_cast<i32>(World::sDimensions.y / 2)) * static_cast<i32>(sDimensions.z)) + i32(z) });*/

				Block::Type bType = (*this)[{ x, y, z }].GetType();

				Block::Texture tex = Block::Map.at(bType);

				m_InstancedBlocks.emplace_back(model, glm::mat3x2{ tex.mTop, tex.mSide, tex.mBottom });
			}
		}
	}

	m_Instanced = true;
	//m_VBO->Bind();
	//glBufferData(GL_ARRAY_BUFFER, static_cast<u32>(m_InstancedBlocks.size() * sizeof(BlockInstance)), m_InstancedBlocks.data(), GL_STATIC_DRAW);
	//m_VBO = std::make_unique<VertexBuffer>(m_InstancedBlocks.data(), static_cast<u32>(m_InstancedBlocks.size() * sizeof(BlockInstance)));
}

void Chunk::Generate()
{
	for (u32 x = 0; x < sDimensions.x; x++)
	{
		for (u32 z = 0; z < sDimensions.z; z++)
		{
			const float noise = (glm::simplex(glm::vec2{ static_cast<i32>(x) + m_WorldPosition.x, static_cast<i32>(z) + m_WorldPosition.y } *0.002f) + 1) / 2.0f;

			for (u32 y = 0; y < sDimensions.y; y++)
			{
				if (y < noise * sDimensions.y)
				{
					if (y < (noise * sDimensions.y) / 3)
						(*this)[{ x, y, z }] = Block(Block::Type::STONE);
					else
						(*this)[{ x, y, z }] = Block(Block::Type::DIRT);
				}
				else
					(*this)[{ x, y, z }] = Block(Block::Type::AIR);
			}
		}
	}
}

void Chunk::Update()
{
	if (m_Instanced)
	{
		m_VBO->Bind();
		glBufferData(GL_ARRAY_BUFFER, static_cast<u32>(m_InstancedBlocks.size() * sizeof(BlockInstance)), m_InstancedBlocks.data(), GL_STATIC_DRAW);
		m_Instanced = false;
	}
		
}

void Chunk::Render(Shader& shader)
{
	//GenerateMesh();

	// mat4 = model matrix, mat3x2 = 3 tiles; top, side and bottom
	//std::mutex mtx;
	//mtx.lock();

	static Mesh cubeMesh = Mesh::Cube();

	cubeMesh.GetVAO().Bind();

	m_VBO->Bind();
	//VertexBuffer vbo(m_InstancedBlocks.data(), static_cast<u32>(m_InstancedBlocks.size() * sizeof(BlockInstance)));

	// model matrix9

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), 0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)sizeof(glm::vec4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	// tiles
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)(sizeof(glm::mat4)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)(sizeof(glm::mat4) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(BlockInstance), (void*)(sizeof(glm::mat4) + (2 * sizeof(glm::vec2))));

	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);

	shader.Bind();

	shader.Set("uProj", Game::Instance()->GetProjection());
	shader.Set("uView", m_World->GetPlayer().GetCamera().GetView());

	Game::Instance()->GetTextureMap().Bind();
	shader.Set("uTexture", glm::uvec1{ 0 });

	glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(cubeMesh.GetVertices().size()), GL_UNSIGNED_INT, cubeMesh.GetIndices().data(),
	                        static_cast<GLsizei>(m_InstancedBlocks.size()));
	//mtx.unlock();

	// chunk debug cube
	/*static Shader debugShader("./res/shaders/debug.vert", "./res/shaders/debug.frag");
	debugShader.Bind();

	debugShader.Set("uProj", Game::Instance()->GetProjection());
	debugShader.Set("uView", m_World->GetPlayer().GetCamera().GetView());

	glm::mat4 model(1.0f);
	model = glm::scale(model , { sBlocks1D, sBlocks1D, sBlocks1D });
	model = glm::translate(model, m_WorldPosition);

	debugShader.Set("uModel", model);

	cubeMesh.GetVAO().Bind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, cubeMesh.GetVertices().size(), GL_UNSIGNED_INT, cubeMesh.GetIndices().data());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
}

void Chunk::GenerateMesh()
{
	MeshData meshData;

	constexpr glm::vec3 vertices[] = {
		// back x
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },

		// front x
		{ 1.0f,  1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f, -1.0f,  1.0f },
	};
	constexpr u8 indices[6][4] = {
		// x-
		{ 0, 1, 2, 3 },
		// y-
		{ 1, 3, 5, 7 },
		// z-
		{ 0, 1, 4, 5 },
		// x+
		{ 4, 5, 6, 7 },
		// y+
		{ 0, 2, 4, 6 },
		// z+
		{ 2, 3, 6, 7 },
	};

	MeshConstructor constructor;
	// loop through each block
	static auto lambda = [&]{
		for (u32 x = 0; x < sDimensions.x; x++)
		{
			for (u32 y = 0; y < sDimensions.y; y++)
			{
				for (u32 z = 0; z < sDimensions.z; z++)
				{
					const glm::uvec3 blockPos = { x, y, z };

					if (!(*this)[blockPos].IsSolid())
						continue;

					// loop through each axis
					// 0 = x-
					// 1 = y-
					// 2 = z-
					// 3 = x+
					// 4 = y+
					// 5 = z+
					for (u8 face = 0; face < 6; face++)
					{
						// 0 = x, 1 = y, 2 = z
						u8 dir = face % 3;
						bool backFace = face < 3;

						// in each axis, check back and front

						// back face
						// check back block
						glm::ivec3 otherBlockPos = blockPos;

						backFace ? otherBlockPos[dir]--
							: otherBlockPos[dir]++;

						//goto skip;

						// if it's a back face, check if it's below 0 in the x or z dir
						// or if it's a front face, check if it's out of range
						if ((otherBlockPos[dir] < 0
							|| otherBlockPos[dir] > (static_cast<i32>(sDimensions[dir]) - 1)) && (dir == 0 || dir == 2))
						{
							glm::ivec2 otherChunkPos = m_LocalPosition;
							if (dir == 0)
								backFace ? otherChunkPos[0]--
								: otherChunkPos[0]++;
							else // if its 2, z coordinate in block coordinate system = y in chunk coord
								backFace ? otherChunkPos[1]--
								: otherChunkPos[1]++;

							// check back chunk
							glm::ivec3 otherChunkBlockPos = blockPos;
							backFace ? otherChunkBlockPos[dir] = static_cast<i32>(sDimensions[dir]) - 1
								: otherChunkBlockPos[dir] = 0;

							if ((*this->m_World)[otherChunkPos][otherChunkBlockPos].IsSolid())
							{
								continue;
							}
							goto skip;
						}

						// if out of range in y
						// since there are no top/bottom chunk, we want to draw it
						if ((otherBlockPos[dir] < 0
							|| otherBlockPos[dir] > static_cast<i32>(sDimensions[dir]) - 1))
							goto skip;

						
						if ((*this)[otherBlockPos].IsSolid())
							continue;

						skip:
						//std::array<Vertex, 4> quad = {};
						std::vector<Vertex> quad;
						for (u8 i = 0; i < 4; i++)
						{
							glm::vec3 normal(0.0f);
							normal[dir] = backFace ? -1.0f : 1.0f;

							/*quad[i] = {glm::vec3(blockPos) + vertices[indices[face][i]], normal, glm::vec2{} };*/
							quad.push_back({ glm::vec3(blockPos) + vertices[indices[face][i]], normal, glm::vec2{} });
						}
						constructor.AddQuad(quad, backFace);
					}

				}
			}
		}
		return 0;
	}(); 

	static Shader shader("./res/shaders/debug.vert", "./res/shaders/debug.frag");
	shader.Bind();

	glm::mat4 model(1.0f);
	model = glm::translate(model, { m_WorldPosition.x, 0, m_WorldPosition.y });
	//model = glm::scale(model, glm::vec3(sDimensions));

	shader.Set("uModel", model);
	shader.Set("uProj", Game::Instance()->GetProjection());
	shader.Set("uView", Game::Instance()->GetWorld().GetPlayer().GetCamera().GetView());

	static Mesh* mesh = constructor.ToMesh();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->Render(shader);

	//delete mesh;
}

//void Chunk::GenerateMesh()
//{
//	MeshConstructor meshConstructor;
//
//	u8 direction, workAxis1, workAxis2;
//
//	std::array<std::array<bool, sBlocks1D>, sBlocks1D> merged;
//
//	glm::vec3 startPos, currPos, quadSize, m, n, offsetPos;
//	std::vector<Vertex> vertices;
//
//	Block* startBlock = nullptr;
//
//	auto IsBlockFaceVisible = [&](glm::vec3 blockPosition, int axis, bool backFace) {
//		blockPosition[axis] += backFace ? -1 : 1;
//		if (blockPosition.x < 0 || blockPosition.x > sBlocks1D - 1
//			|| blockPosition.y < 0 || blockPosition.y > sBlocks1D - 1
//			|| blockPosition.z < 0 || blockPosition.z > sBlocks1D - 1)
//			return true;
//		return !(*this)[blockPosition].IsSolid();
//	};
//
//	auto CompareStep = [&](glm::vec3 a, glm::vec3 b, int direction, bool backFace) {
//		Block& blockA = (*this)[a];
//		Block& blockB = (*this)[b];
//
//		return blockA == blockB && blockB.IsSolid() && IsBlockFaceVisible(b, direction, backFace);
//	};
//
//	// Iterate over each face of the blocks.
//	for (u8 face = 0; face < 6; face++) 
//	{
//		bool isBackFace = face > 2;
//
//		direction = face % 3;
//		workAxis1 = (direction + 1) % 3;
//		workAxis2 = (direction + 2) % 3;
//
//		startPos = glm::vec3();
//		currPos = glm::vec3();
//
//		// Iterate over the chunk layer by layer.
//		for (startPos[direction] = 0; startPos[direction] < sBlocks1D; startPos[direction]++)
//		{
//			memset(merged.data(), 0, sizeof(merged));
//
//			// Build the slices of the mesh.
//			for (startPos[workAxis1] = 0; startPos[workAxis1] < sBlocks1D; startPos[workAxis1]++) {
//				for (startPos[workAxis2] = 0; startPos[workAxis2] < sBlocks1D; startPos[workAxis2]++) {
//					startBlock = &(*this)[startPos];
//
//					// If this block has already been merged, is air, or not visible skip it.
//					if (merged[startPos[workAxis1]][startPos[workAxis2]] || !startBlock->IsSolid() || !IsBlockFaceVisible(startPos, direction, isBackFace)) 
//					{
//						continue;
//					}
//
//					// reset work var
//					quadSize = glm::vec3();
//
//					// Figure out the width, then save it
//					for (currPos = startPos, currPos[workAxis2]++; currPos[workAxis2] < sBlocks1D && CompareStep(startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]]; currPos[workAxis2]++) {}
//					quadSize[workAxis2] = currPos[workAxis2] - startPos[workAxis2];
//
//					// Figure out the height, then save it
//					for (currPos = startPos, currPos[workAxis1]++; currPos[workAxis1] < sBlocks1D && CompareStep(startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]]; currPos[workAxis1]++) {
//						for (currPos[workAxis2] = startPos[workAxis2]; currPos[workAxis2] < sBlocks1D && CompareStep(startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]]; currPos[workAxis2]++) {}
//
//						// If we didn't reach the end then its not a good add.
//						if (currPos[workAxis2] - startPos[workAxis2] < quadSize[workAxis2]) {
//							break;
//						}
//						else {
//							currPos[workAxis2] = startPos[workAxis2];
//						}
//					}
//					quadSize[workAxis1] = currPos[workAxis1] - startPos[workAxis1];
//
//					// Now we add the quad to the mesh
//					m = glm::vec3();
//					m[workAxis1] = quadSize[workAxis1];
//					n = glm::vec3();
//					n[workAxis2] = quadSize[workAxis2];
//
//					// We need to add a slight offset when working with front faces.
//					offsetPos = startPos;
//					offsetPos[direction] += isBackFace ? 0 : 1;
//
//					// cant use unordered map with pair, throws compile time error
//					std::unordered_map<u8, glm::vec3> normals = {
//						{ 0, { isBackFace ? -1.0f : 1.0f, 0.0f, 0.0f } },
//						{ 1, { 0.0f, isBackFace ? -1.0f : 1.0f, 0.0f } },
//						{ 2, { 0.0f, 0.0f, isBackFace ? -1.0f : 1.0f } },
//					};
//
//					glm::vec3 normal = normals[direction];
//					glm::vec2 uvOffset(0.0f);
//
//					// side
//					//if (normal.x > 0.0f || normal.x < 0.0f || normal.z > 0.0f || normal.z < 0.0f)
//					//{
//					//	uvOffset = (Block::Map.at(startBlock->GetType()).mSide * 64.0f) / 1024.0f;
//					//}
//					//// top
//					//else if (normal.y > 0.0f)
//					//{
//					//	uvOffset = (Block::Map.at(startBlock->GetType()).mTop * 64.0f) / 1024.0f;
//					//}	
//					//// bottom
//					//else if (normal.y < 0.0f)
//					//{
//					//	uvOffset = (Block::Map.at(startBlock->GetType()).mBottom * 64.0f) / 1024.0f;
//					//}
//					
//					if (normal.x > 0.0f || normal.x < 0.0f || normal.z > 0.0f || normal.z < 0.0f)
//					{
//						uvOffset = Block::Map.at(startBlock->GetType()).mSide;
//					}
//					// top
//					else if (normal.y > 0.0f)
//					{
//						uvOffset = Block::Map.at(startBlock->GetType()).mTop;
//					}
//					// bottom
//					else if (normal.y < 0.0f)
//					{
//						uvOffset = Block::Map.at(startBlock->GetType()).mBottom;
//					}
//
//					glm::vec2 factor = isBackFace ? 
//						glm::vec2{ quadSize[workAxis1], quadSize[workAxis2] } : glm::vec2{ quadSize[workAxis2], quadSize[workAxis1] };
//
//					vertices = {
//						{ offsetPos, normal, { 1.0f, 1.0f }, uvOffset, factor },
//						{ offsetPos + m, normal, { 1.0f, 0.0f }, uvOffset, factor },
//						{ offsetPos + m + n, normal, { 0.0f, 0.0f }, uvOffset, factor },
//						{ offsetPos + n, normal, { 0.0f, 1.0f }, uvOffset, factor }
//					};
//
//					if (normal.z > 0.0f)
//						vertices = {
//							{ offsetPos, normal, { 0.0f, 0.0f }, uvOffset, factor },
//							{ offsetPos + m, normal, { 1.0f, 0.0f }, uvOffset, factor },
//							{ offsetPos + m + n, normal, { 1.0f, 1.0f }, uvOffset, factor },
//							{ offsetPos + n, normal, { 0.0f, 1.0f }, uvOffset, factor }
//					};
//
//					/*vertices = {
//						{ offsetPos, normal, { 0.0f, 0.0f }, uvOffset },
//						{ offsetPos + m, normal, { 1.0f, 0.0f }, uvOffset },
//						{ offsetPos + m + n, { 1.0f, 1.0f }, uvOffset },
//						{ offsetPos + n, normal, { 0.0f, 1.0f }, uvOffset }
//					};*/
//
//					meshConstructor.AddQuad(vertices, isBackFace);
//
//					// Mark it merged
//					for (int f = 0; f < quadSize[workAxis1]; f++) {
//						for (int g = 0; g < quadSize[workAxis2]; g++) {
//							merged[startPos[workAxis1] + f][startPos[workAxis2] + g] = true;
//						}
//					}
//				}
//			}
//		}
//	}
//
//	m_Mesh = meshConstructor.ToMesh();
//}

//void Chunk::GenerateMesh()
//{	
//	auto airBlockAround = [&](glm::uvec3 position)
//	{
//		if (position.x == 0 || position.x == sBlocks1D - 1 || position.y == 0 || position.y == sBlocks1D - 1 || position.z == 0 || position.z == sBlocks1D - 1)
//			return true;
//
//		for (i8 x = -1; x <= 1; x++)
//		{
//			for (i8 y = -1; y <= 1; y++)
//			{
//				for (i8 z = -1; z <= 1; z++)
//				{
//					glm::uvec3 pos = { position.x + x, position.y + y, position.z + z };
//					if (m_Blocks[pos.x + sBlocks1D * (pos.y + sBlocks1D * pos.z)]->GetType() == Block::Type::AIR)
//						return true;
//				}
//			}
//		}
//		return false;
//	};
//
//	std::vector<Vertex> vertices;
//
//	// will contain position
//	glm::uvec3 startingBlock, endingBlock;
//
//	std::vector<u32> visitedBlocks;
//	// Iterate through the blocks
//	for (u32 x = 0; x < sBlocks1D; x++)
//	{
//		for (u32 y = 0; y < sBlocks1D; y++)
//		{
//			for (u32 z = 0; z < sBlocks1D; z++)
//			{
//				// find index in 3d
//				const u32 startingIndex = x + sBlocks1D * (y + sBlocks1D * z);
//				
//				// skip if block is air
//				if (m_Blocks[startingIndex]->GetType() == Block::Type::AIR 
//					|| std::find(visitedBlocks.cbegin(), visitedBlocks.cend(), startingIndex) != visitedBlocks.cend()
//					|| !airBlockAround(glm::uvec3(x, y ,z))) continue;
//
//				startingBlock = glm::vec3(x, y, z);
//				endingBlock = startingBlock;
//				visitedBlocks.push_back(startingIndex);
//
//				// find length in x axis
//				u32 xLength;
//				for (u32 x1 = startingBlock.x + 1; x1 < sBlocks1D; x1++)
//				{
//					const u32 index = x1 + sBlocks1D * (y + sBlocks1D * z);
//					if (m_Blocks[index]->GetType() != m_Blocks[startingIndex]->GetType()
//						|| std::find(visitedBlocks.cbegin(), visitedBlocks.cend(), index) != visitedBlocks.cend()
//						|| !airBlockAround(glm::uvec3(x1, y, z))) break;
//
//					endingBlock.x = x1;
//					visitedBlocks.push_back(index);
//				}
//				xLength = endingBlock.x;
//
//				// find length in y axis
//				u32 yLength;
//				for (u32 y2 = startingBlock.y + 1; y2 < sBlocks1D; y2++)
//				{
//					bool usable = true;
//
//					for (u32 x2 = startingBlock.x; x2 <= xLength; x2++)
//					{
//						const u32 index = x2 + sBlocks1D * (y2 + sBlocks1D * z);
//						if (m_Blocks[index]->GetType() != m_Blocks[startingIndex]->GetType()
//							|| std::find(visitedBlocks.cbegin(), visitedBlocks.cend(), index) != visitedBlocks.cend()
//							|| !airBlockAround(glm::uvec3(x2, y2, z)))
//						{
//							usable = false;
//							break;
//						}
//					}
//
//					if (!usable) break;
//
//					for (u32 x2 = startingBlock.x; x2 <= xLength; x2++)
//					{
//						const u32 index = x2 + sBlocks1D * (y2 + sBlocks1D * z);
//						visitedBlocks.push_back(index);
//					}
//
//					endingBlock.y = y2;
//				}
//				yLength = endingBlock.y;
//
//				// find length in z axis
//				for (u32 z3 = startingBlock.z + 1; z3 < sBlocks1D; z3++)
//				{
//					bool usable = true;
//
//					for (u32 y3 = startingBlock.y; y3 <= yLength; y3++)
//					{
//						for (u32 x3 = startingBlock.x; x3 <= xLength; x3++)
//						{
//							const u32 index = x3 + sBlocks1D * (y3 + sBlocks1D * z3);
//							if (m_Blocks[index]->GetType() != m_Blocks[startingIndex]->GetType()
//								|| std::find(visitedBlocks.cbegin(), visitedBlocks.cend(), index) != visitedBlocks.cend()
//								|| !airBlockAround(glm::uvec3(x3, y3, z3)))
//							{
//								usable = false;
//								goto outer;
//							}
//						}
//					}
//
//				outer:
//
//					if (!usable) break;
//
//					for (u32 y3 = startingBlock.y; y3 <= yLength; y3++)
//					{
//						for (u32 x3 = startingBlock.x; x3 <= xLength; x3++)
//						{
//							const u32 index = x3 + sBlocks1D * (y3 + sBlocks1D * z3);
//							visitedBlocks.push_back(index);
//						}
//					}
//
//					endingBlock.z = z3;
//				}
//
//				glm::vec3 corners[8] = {
//					// top
//					{ startingBlock.x, startingBlock.y, startingBlock.z },
//					{ startingBlock.x, startingBlock.y, endingBlock.z },
//					{ endingBlock.x, startingBlock.y, startingBlock.z },
//					{ endingBlock.x, startingBlock.y, endingBlock.z },
//					
//					// bottom
//					{ startingBlock.x, endingBlock.y, startingBlock.z },
//					{ startingBlock.x, endingBlock.y, endingBlock.z },
//					{ endingBlock.x, endingBlock.y, startingBlock.z },
//					{ endingBlock.x, endingBlock.y, endingBlock.z }
//				};
//
//				// faces
//				// top
//				vertices.emplace_back(corners[0], 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
//				vertices.emplace_back(corners[1], 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
//				vertices.emplace_back(endingBlock.z - startingBlock.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
//
//			}
//		}
//	}
//}