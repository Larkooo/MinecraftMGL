#pragma once

#include <vector>
#include <array>

#include "Definitions.h"

class Mesh;

class MeshData
{
	std::vector<Vertex> m_Vertices;
	std::vector<u32> m_Indices;

public:
	MeshData() = default;

	void AddQuad(const std::array<Vertex, 4>& vertices);

	Mesh* ToMesh();
};

