#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Game.h>

class Camera
{
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Direction;

	float m_Fov;

	glm::vec3 m_Up;
	glm::vec3 m_Right;

	glm::vec3 m_WorldUp;
	
public:
	Camera(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 rotation = { -90.0f, 0.0f, 0.0f }, glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f }, float fov = 90.0f);

	// view mat
	glm::mat4 GetView();

	// used for front/back movements
	glm::vec3 GetDirection() { return m_Direction; }
	// used for left/right movements
	glm::vec3 GetRight() { return m_Right; }

	glm::vec3 GetPosition() { return m_Position; }
	void SetPosition(glm::vec3 position) { m_Position = position; }

	void Update();

private:
	void UpdateVectors();
};

