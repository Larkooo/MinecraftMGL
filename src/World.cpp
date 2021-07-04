#include "Core/World.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"

#include <thread>

void World::Init()
{
	m_Chunks.fill({});
}

void World::HandleEvents()
{
	m_Player.HandleEvents();
}

void World::Update()
{
	m_Player.Update();
}

void World::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader shader("./res/shaders/triangle.vert", "./res/shaders/triangle.frag");
	
	Mesh cube = Mesh::Cube();
	
	shader.Bind();

	shader.Set("uProj", Game::Instance()->GetProjection());
	shader.Set("uView", Game::Instance()->GetWorld().GetPlayer().GetCamera().GetView());

	static auto* modelMatrices = new std::array<glm::mat4, sChunks* Chunk::sBlocks>;

	static auto fill = [&]() {
		modelMatrices->fill(glm::mat4{ 0.0f });
		return 0;
	}();

	// disaster code
	// chunks
	for (size_t n = 0; n < 4; n++)
	{
		for (size_t m = 0; m < 4; m++)
		{
			for (size_t k = 0; k < 4; k++)
			{
				size_t chunkIndex = n + (m * 4) + (k * 4 * 4);

				//std::cout << chunkIndex << std::endl;
				//static const auto chunkLambda = [&]()
				{
					// blocks
					for (size_t x = 0; x < 16; x++)
					{
						for (size_t y = 0; y < 16; y++)
						{
							for (size_t z = 0; z < 16; z++)
							{
								size_t blockIndex = x + (y * 16) + (z * 16 * 16);

								glm::vec3 pos = glm::vec3(n + x, m + y, k + z);

								glm::mat4 model(1.0f);
								model = glm::translate(model, pos);

								modelMatrices->at(chunkIndex + blockIndex) = model;
								//std::cout << chunkIndex + blockIndex << std::endl;
							}
						}
					}


				};
				//m_Chunks[chunkIndex].mThread = new std::thread(chunkLambda);
			}
		}
	}

	//for (Chunk& c : m_Chunks)
	//	(*c.mThread).join();

	cube.GetVAO().Bind();

	VertexBuffer vbo(modelMatrices, sizeof(*modelMatrices));
	vbo.Bind();

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)sizeof(glm::vec4));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	vbo.Unbind();

	glDrawElementsInstanced(GL_TRIANGLES, cube.GetIndices().size(), GL_UNSIGNED_INT, cube.GetIndices().data(), modelMatrices->size());

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << err << std::endl;
	}

	shader.Unbind();
}
