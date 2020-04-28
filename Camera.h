#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();

	void move(GLFWwindow* window, float deltaTime);

	void rotateCamera(GLFWwindow *window);

	glm::mat4 getTransform();

	~Camera();
private:
	glm::vec3 pos, front, up, right;
	float pitch, yaw;

	float lastMouseX, lastMouseY;

	float velocity = 1.5f;
	float sensitivity = 0.1f;

	const static glm::vec3 WORLD_UP;

	void updateFront();
};

