#include "Game/Player.h"

Player::Player(glm::vec3 position, glm::vec3 velocity) : m_Position(position), m_Velocity(velocity)
{
	m_Camera.SetPosition(m_Position);
}

void Player::HandleEvents()
{
	const float deltaTime = Game::Instance()->GetDeltaTime().count();
	GLFWwindow* window = Game::Instance()->GetWindow().GetGLFWWindow();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_Velocity += m_Camera.GetDirection() * 50.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_Velocity -= m_Camera.GetDirection() * 50.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_Velocity -= m_Camera.GetRight() * 50.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_Velocity += m_Camera.GetRight() * 50.0f;

	m_Velocity *= deltaTime;
}

void Player::Update()
{
	m_Position += m_Velocity;

	m_Camera.SetPosition(m_Position + glm::vec3(0.0f, 0.5f, 0.0f));
	// Update rotation
	m_Camera.Update();

	// std::cout << m_Position.x << " " << m_Position.y << " " << m_Position.z << std::endl;
} 

void Player::Render()
{
}
