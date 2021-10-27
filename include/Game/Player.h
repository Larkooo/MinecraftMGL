#pragma once

#include <Core/Game.h>

#include <Graphics/Camera.h>

class Chunk;

class Player
{
	glm::vec3 m_Position;
	glm::vec3 m_LastPosition;

	glm::vec3 m_Velocity;

	Camera m_Camera;

	Chunk* m_CurrentChunk = nullptr;
	
public:
	Player(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 velocity = { 0.0f, 0.0f, 0.0f });

	glm::vec3 GetPosition() const { return m_Position; }
	void SetPosition(glm::vec3 pos) { m_Position = pos; }

	glm::vec3 GetVelocity() const { return m_Velocity; }
	Camera& GetCamera() { return m_Camera; }

	void HandleEvents();
	void Update();
	void Render();
};

