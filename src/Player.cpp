#include "Game/Player.h"

Player::Player(glm::vec3 position, glm::vec3 velocity) : m_Position(position), m_Velocity(velocity)
{
	m_Camera.SetPosition(m_Position);
}

void Player::HandleEvents()
{
}

void Player::Update()
{
	GLFWwindow* window = Game::Instance()->GetWindow().GetGLFWWindow();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_Position += m_Camera.GetDirection() * 0.1f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_Position -= m_Camera.GetDirection() * 0.1f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_Position -= m_Camera.GetRight() * 0.1f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_Position += m_Camera.GetRight() * 0.1f;

	m_Camera.SetPosition(m_Position + glm::vec3(0.0f, 0.5f, 0.0f));
	// Update rotation
	m_Camera.Update();
}

void Player::Render()
{
}
