#include "Camera.h"

const glm::vec3 Camera::WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() {
	pos = glm::vec3(0.0f, 3.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = -45.0f;
	yaw = 0.0f;

	lastMouseX = 0.0f;
	lastMouseY = 0.0f;

	updateFront();
}

glm::mat4 Camera::getTransform() {
	return glm::lookAt(pos, pos + front, up);
}

void Camera::updateFront() {
	glm::vec3 newFront = glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, WORLD_UP));
	up = glm::normalize(glm::cross(right, front));

}

void Camera::move(GLFWwindow* window, float deltaTime) {
	float amt = velocity * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W)) {
		pos += front * amt;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		pos -= front * amt;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		pos -= right * amt;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		pos += right * amt;
	}
}

void Camera::rotateCamera(GLFWwindow *window) {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	float deltaX = lastMouseX - mouseX;
	float deltaY = lastMouseY - mouseY;
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	pitch += deltaY;
	yaw -= deltaX;

	if (pitch > 89.9f) pitch = 89.9f;
	if (pitch < -89.9f) pitch = -89.9f;

	updateFront();
}

Camera::~Camera() {

}