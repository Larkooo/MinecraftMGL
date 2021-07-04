#include "Graphics\Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 rotation, glm::vec3 worldUp, float fov) : m_Position(position), m_Rotation(rotation), m_WorldUp(worldUp), m_Fov(fov)
{
	UpdateVectors();
}

void Camera::Update()
{
	static Window& window = Game::Instance()->GetWindow();
	static glm::vec2 lastMouse(window.GetWidth() / 2.0f, window.GetHeight() / 2.0f);
	static bool firstMouse = true;

	/*if (showCursor)
	{
		lastMouse = { xpos, ypos };
		return;
	}*/

	glm::dvec2 pos;
	glfwGetCursorPos(window.GetGLFWWindow(), &pos.x, &pos.y);
	glm::vec2 offset;

	if (firstMouse)
	{
		lastMouse = pos;
		firstMouse = false;
	}

	offset.x = (pos.x - lastMouse.x) * 0.3f;
	offset.y = (lastMouse.y - pos.y) * 0.3f;

	lastMouse = pos;

	// Update rotation
	m_Rotation.x += offset.x, m_Rotation.y += offset.y;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_Rotation.y > 89.0f)
		m_Rotation.y = 89.0f;
	else if (m_Rotation.y < -89.0f)
		m_Rotation.y = -89.0f;

	// Update vectors
	UpdateVectors();

	//std::cout << m_Rotation.x << " " << m_Rotation.y << " " << m_Rotation.z << std::endl;
}

void Camera::UpdateVectors()
{
	// Update vectors
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
	tempFront.y = sin(glm::radians(m_Rotation.y));
	tempFront.z = sin(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));

	m_Direction = glm::normalize(tempFront);

	m_Right = glm::normalize(glm::cross(m_Direction, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Direction));
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
}
