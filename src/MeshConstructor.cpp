#include "Graphics/MeshConstructor.h"

#include <Graphics/Mesh.h>

void MeshConstructor::AddQuad(const std::vector<Vertex>& vertices, bool isBackFace)
{
	if (vertices.size() != 4) {
		std::cout << "A quad requires 4 vertices" << std::endl;
		return;
	}

	// Add the 4 vertices, and color for each vertex.
	for (int i = 0; i < vertices.size(); i++) {
		m_Vertices.emplace_back(vertices[i]);
	}

	if (!isBackFace) {
		m_Indices.push_back(m_Vertices.size() - 4);
		m_Indices.push_back(m_Vertices.size() - 3);
		m_Indices.push_back(m_Vertices.size() - 2);

		m_Indices.push_back(m_Vertices.size() - 2);
		m_Indices.push_back(m_Vertices.size() - 1);
		m_Indices.push_back(m_Vertices.size() - 4);
	}	  
	else {
		m_Indices.push_back(m_Vertices.size() - 4);
		m_Indices.push_back(m_Vertices.size() - 1);
		m_Indices.push_back(m_Vertices.size() - 2);
											  
		m_Indices.push_back(m_Vertices.size() - 2);
		m_Indices.push_back(m_Vertices.size() - 3);
		m_Indices.push_back(m_Vertices.size() - 4);
	}

}

Mesh* MeshConstructor::ToMesh()
{
	return new Mesh(m_Vertices, m_Indices);
}
