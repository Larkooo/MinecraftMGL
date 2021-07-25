#pragma once

#include "Definitions.h"

#include <vector>

class Mesh;

class MeshConstructor
{
	std::vector<Vertex> m_Vertices;
	std::vector<u32> m_Indices;

public:
	MeshConstructor() = default;

	void AddQuad(const std::vector<Vertex>& vertices, bool isBackFace);

	Mesh* ToMesh();
};

