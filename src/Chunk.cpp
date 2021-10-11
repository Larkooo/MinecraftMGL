#include "Game/Chunk.h"

#include "Graphics/MeshData.h"
#include "Core/Game.h"
#include "Core/World.h"

#include <functional>
#include <array>

#include <glm/gtc/noise.hpp>

Chunk::Chunk(World* world, glm::vec3 pos) : m_Position(pos), m_World(world)
{
	for (u32 i = 0; i < sBlocks1D * sBlocks1D * sBlocks1D; i++)
		m_Blocks[i] = Block();
}

void Chunk::InstantiateBlocks()
{
	auto airBlockAround = [&](glm::vec3 position)
	{
		// 0 = x, 1 = y, 2 = z
		for (u8 axis = 0; axis < 3; axis++)
		{
			glm::vec3 frontBlock = position, backBlock = position;
			frontBlock[axis]++, backBlock[axis]--;

			for (u8 saxis = 0; saxis < 3; saxis++)
			{
				glm::vec3 otherChunk, otherBlock;
				if (frontBlock[saxis] < 0)
				{
					otherChunk = m_Position; otherChunk[saxis]--;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = frontBlock; otherBlock[saxis] = sBlocks1D - 1;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[frontBlock].IsSolid())
						return true;
					goto skip;
				}
				else if (frontBlock[saxis] > sBlocks1D - 1)
				{
					otherChunk = m_Position; otherChunk[saxis]++;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = frontBlock; otherBlock[saxis] = 0;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[backBlock].IsSolid())
						return true;
					goto skip;
				}

				else if (backBlock[saxis] < 0)
				{
					otherChunk = m_Position; otherChunk[saxis]--;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = backBlock; otherBlock[saxis] = sBlocks1D - 1;
					if (!(*m_World)[otherChunk][otherBlock].IsSolid())
						return true;

					if (!(*this)[frontBlock].IsSolid())
						return true;
					goto skip;
				}
				else if (backBlock[saxis] > sBlocks1D - 1)
				{
					otherChunk = m_Position; otherChunk[saxis]++;
					if (otherChunk[saxis] < 0 || otherChunk[saxis] > World::sDimensions[saxis] - 1)
						return true;

					otherBlock = backBlock; otherBlock[saxis] = 0;
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

	for (u32 x = 0; x < sBlocks1D; x++)
	{
		for (u32 y = 0; y < sBlocks1D; y++)
		{
			for (u32 z = 0; z < sBlocks1D; z++)
			{
				if (!airBlockAround({ x, y, z }) || !(*this)[{ x, y, z }].IsSolid())
					continue;

				glm::mat4 model(1.0f);
				model = glm::translate(model, { (m_Position.x * sBlocks1D) + x, (m_Position.y * sBlocks1D) + y, (m_Position.z * sBlocks1D) + z });

				Block::Type bType = (*this)[{ x, y, z }].GetType();

				Block::Texture tex = Block::Map.at(bType);

				m_InstancedBlocks.emplace_back(model, glm::mat3x2{ tex.mTop, tex.mSide, tex.mBottom });
			}
		}
	}
}

void Chunk::Update()
{
}

void Chunk::Render(Shader& shader)
{
	//GenerateMesh();

	// mat4 = model matrix, mat3x2 = 3 tiles, top, side and bottom
	using BlockInstance = std::pair<glm::mat4, glm::mat3x2>;

	static Mesh cubeMesh = Mesh::Cube();

	cubeMesh.GetVAO().Bind();

	VertexBuffer buffer(m_InstancedBlocks.data(), m_InstancedBlocks.size() * sizeof(BlockInstance));

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

	glDrawElementsInstanced(GL_TRIANGLES, cubeMesh.GetVertices().size(), GL_UNSIGNED_INT, cubeMesh.GetIndices().data(),
	                        m_InstancedBlocks.size());

	// chunk debug cube
	static Shader debugShader("./res/shaders/debug.vert", "./res/shaders/debug.frag");
	debugShader.Bind();

	debugShader.Set("uProj", Game::Instance()->GetProjection());
	debugShader.Set("uView", m_World->GetPlayer().GetCamera().GetView());

	glm::mat4 model(1.0f);
	model = glm::scale(model , { sBlocks1D, sBlocks1D, sBlocks1D });
	model = glm::translate(model, m_Position + (7.0f / static_cast<float>(sBlocks1D)));

	debugShader.Set("uModel", model);

	cubeMesh.GetVAO().Bind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, cubeMesh.GetVertices().size(), GL_UNSIGNED_INT, cubeMesh.GetIndices().data());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Chunk::GenerateMesh()
{
	MeshData meshData;

	// loop through voxels
	for (u32 x = 0; x < sBlocks1D; x++)
	{
		for (u32 y = 0; y < sBlocks1D; y++)
		{
			for (u32 z = 0; z < sBlocks1D; z++)
			{
				// faces vertices
				const Vertex cornersVertices[8] = {
					// bottom left back
					{ { x, y, z }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
					// bottom right back
					{ { x + 1.0f, y, z }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
					// top left back
					{ { x, y + 1.0f, z }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
					// top right back
					{ { x + 1.0f, y + 1.0f, z }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },

					// bottom left front
					{ { x, y, z + 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
					// bottom right front
					{ { x + 1.0f, y, z + 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
					// top left front
					{ { x, y + 1.0f, z + 1.0f}, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
					// top right front
					{ { x + 1.0f, y + 1.0f, z + 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
				};

				const u8 facesIndices[6][4] = {
					{ 0, 1, 2, 3 },
					{ 4, 5, 6, 7 },
					{ 0, 4, 2, 6 },
					{ 1, 5, 3, 7 },
					{ 2, 3, 6, 7 },
					{ 7, 6, 3, 2 },
				};

				for (u8 face = 0; face < 6; face++)
				{
					auto vertices = std::array<Vertex, 4>();
					for (u8 i = 0; i < 4; i++)
						vertices[i] = cornersVertices[facesIndices[face][i]];

					meshData.AddQuad(vertices);
				}
			}
		}
	}
	static Shader shader("./res/shaders/debug.vert", "./res/shaders/debug.frag");
	shader.Bind();
	shader.Set("uModel", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));

	meshData.ToMesh()->Render(shader);
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