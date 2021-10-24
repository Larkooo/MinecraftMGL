#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Game.h>

struct Plane
{
	glm::vec3 normal;
	float distance;
};

struct Frustum
{
	Plane top;
	Plane left;
	Plane right;
	Plane bottom;
	Plane near;
	Plane far;
};

class Camera
{
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Direction;

	float m_zNear;
	float m_zFar;
	float m_Fov;

	glm::vec3 m_Up;
	glm::vec3 m_Right;

	glm::vec3 m_WorldUp;
	
public:
	Camera(
		glm::vec3 position = { 0.0f, 0.0f, 0.0f }, 
		glm::vec3 rotation = { -90.0f, 0.0f, 0.0f }, 
		glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f }, 
		float fov = 70.0f, float near = 0.1f, float far = 10000.0f);

	// view mat
	glm::mat4 GetView() const;

	// used for front/back movements
	glm::vec3 GetDirection() const { return m_Direction; }
	// used for left/right movements
	glm::vec3 GetRight() const { return m_Right; }

	float GetFov() const { return m_Fov; }
	float GetNear() const { return m_zNear; }
	float GetFar() const { return m_zFar; }

	glm::vec3 GetPosition() const { return m_Position; }
	void SetPosition(glm::vec3 position) { m_Position = position; }

	void Update();

private:
	void UpdateVectors();
};

