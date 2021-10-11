#include "Graphics/MeshData.h"
#include "Graphics/Mesh.h"

#include <iostream>

void MeshData::AddQuad(const std::array<Vertex, 4>& vertices)
{
	for (const Vertex& v : vertices)
		m_Vertices.emplace_back(v);

	size_t vertSize = m_Vertices.size();

	m_Indices.push_back(vertSize - 1);
	m_Indices.push_back(vertSize - 2);
	m_Indices.push_back(vertSize - 4);

	m_Indices.push_back(vertSize - 1);
	m_Indices.push_back(vertSize - 4);
	m_Indices.push_back(vertSize - 3);
}

Mesh* MeshData::ToMesh()
{
	return new Mesh(m_Vertices, m_Indices);
}
