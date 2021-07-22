#include "Game/Chunk.h"
#include "Graphics/MeshConstructor.h"

#include <functional>

Chunk::Chunk()
{
	srand(time(0));
	for (size_t i = 0; i < sBlocks1D * sBlocks1D * sBlocks1D; i++)
	{
		m_Blocks[i] = new Block((Block::Type) (rand() % 3));
	}
}

void Chunk::GenerateMesh()
{
	MeshConstructor meshConstructor;

	u8 direction, workAxis1, workAxis2;

	std::array<std::array<bool, sBlocks1D>, sBlocks1D> merged;

	glm::vec3 startPos, currPos, quadSize, m, n, offsetPos;
	std::vector<Vertex> vertices;

	Block* startBlock = nullptr;

	auto IsBlockFaceVisible = [&](glm::vec3 blockPosition, int axis, bool backFace) {
		blockPosition[axis] += backFace ? -1 : 1;
		if (blockPosition.x < 0 || blockPosition.x > sBlocks1D - 1
			|| blockPosition.y < 0 || blockPosition.y > sBlocks1D - 1
			|| blockPosition.z < 0 || blockPosition.z > sBlocks1D - 1)
			return true;
		return !(*this)[blockPosition].IsSolid();
	};

	auto CompareStep = [&](glm::vec3 a, glm::vec3 b, int direction, bool backFace) {
		Block& blockA = (*this)[a];
		Block& blockB = (*this)[b];

		return blockA == blockB && blockB.IsSolid() && IsBlockFaceVisible(b, direction, backFace);
	};

	// Iterate over each face of the blocks.
	for (u8 face = 0; face < 6; face++) 
	{
		bool isBackFace = face > 2;

		direction = face % 3;
		workAxis1 = (direction + 1) % 3;
		workAxis2 = (direction + 2) % 3;

		startPos = glm::vec3();
		currPos = glm::vec3();

		// Iterate over the chunk layer by layer.
		for (startPos[direction] = 0; startPos[direction] < sBlocks1D; startPos[direction]++)
		{
			memset(merged.data(), 0, sizeof(merged));

			// Build the slices of the mesh.
			for (startPos[workAxis1] = 0; startPos[workAxis1] < sBlocks1D; startPos[workAxis1]++) {
				for (startPos[workAxis2] = 0; startPos[workAxis2] < sBlocks1D; startPos[workAxis2]++) {
					startBlock = &(*this)[startPos];

					// If this block has already been merged, is air, or not visible skip it.
					if (merged[startPos[workAxis1]][startPos[workAxis2]] || !startBlock->IsSolid() || !IsBlockFaceVisible(startPos, direction, isBackFace)) 
					{
						continue;
					}

					// reset work var
					quadSize = glm::vec3();

					// Figure out the width, then save it
					for (currPos = startPos, currPos[workAxis2]++; currPos[workAxis2] < sBlocks1D && CompareStep(startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]]; currPos[workAxis2]++) {}
					quadSize[workAxis2] = currPos[workAxis2] - startPos[workAxis2];

					// Figure out the height, then save it
					for (currPos = startPos, currPos[workAxis1]++; currPos[workAxis1] < sBlocks1D && CompareStep(startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]]; currPos[workAxis1]++) {
						for (currPos[workAxis2] = startPos[workAxis2]; currPos[workAxis2] < sBlocks1D && CompareStep(startPos, currPos, direction, isBackFace) && !merged[currPos[workAxis1]][currPos[workAxis2]]; currPos[workAxis2]++) {}

						// If we didn't reach the end then its not a good add.
						if (currPos[workAxis2] - startPos[workAxis2] < quadSize[workAxis2]) {
							break;
						}
						else {
							currPos[workAxis2] = startPos[workAxis2];
						}
					}
					quadSize[workAxis1] = currPos[workAxis1] - startPos[workAxis1];

					// Now we add the quad to the mesh
					m = glm::vec3();
					m[workAxis1] = quadSize[workAxis1];
					n = glm::vec3();
					n[workAxis2] = quadSize[workAxis2];

					// We need to add a slight offset when working with front faces.
					offsetPos = startPos;
					offsetPos[direction] += isBackFace ? 0 : 1;

					//Draw the face to the mesh
					vertices = {
						{ offsetPos },
						{ offsetPos + m },
						{ offsetPos + m + n },
						{ offsetPos + n }
					};

					meshConstructor.AddQuad(vertices, isBackFace);

					// Mark it merged
					for (int f = 0; f < quadSize[workAxis1]; f++) {
						for (int g = 0; g < quadSize[workAxis2]; g++) {
							merged[startPos[workAxis1] + f][startPos[workAxis2] + g] = true;
						}
					}
				}
			}
		}
	}

	m_Mesh = meshConstructor.ToMesh();
}

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

void Chunk::Update()
{
}

void Chunk::Render(Shader& shader)
{
	m_Mesh->Render(shader);
}
