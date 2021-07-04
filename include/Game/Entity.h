#pragma once

#include <glm/glm.hpp>

class Entity
{
protected:
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;

public:
	Entity(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 velocity = { 0.0f, 0.0f, 0.0f });
};

