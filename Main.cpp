#define GLEW_STATIC

#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Terrain.h"
#include "Camera.h"

GLFWwindow* window;

Camera* camera;
Terrain* terrain;

const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;

float currentTime = 0.0f, lastTime = 0.0f;

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terrain", NULL, NULL);
	glfwMakeContextCurrent(window);
}

int initGL() {
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		std::cin.get();
		return 1;
	}
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glPatchParameteri(GL_PATCH_DEFAULT_OUTER_LEVEL, 5);
	glPatchParameteri(GL_PATCH_DEFAULT_INNER_LEVEL, 5);
}

void tick() {
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

	currentTime = glfwGetTime();

	camera->move(window, currentTime - lastTime);
	camera->rotateCamera(window);

	lastTime = currentTime;

	glfwSetCursorPos(window, 0.0, 0.0);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	terrain->render(camera);

	glfwSwapBuffers(window);
}

int main() {
	initGLFW();
	initGL();

	camera = new Camera();
	terrain = new Terrain(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (!glfwWindowShouldClose(window)) {
		tick();
		render();
	}
	return 0;
}