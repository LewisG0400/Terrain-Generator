//Using https://github.com/Auburns/FastNoise/wiki

#define GLEW_STATIC

#include <iostream>
#include <time.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Terrain.h"
#include "Camera.h"

GLFWwindow* window;

Camera* camera;
Terrain* terrain;

bool fullscreen = false;
const int WINDOW_WIDTH_SMALL = 1024, WINDOW_HEIGHT_SMALL = 768;
int WINDOW_WIDTH_FULLSCREEN, WINDOW_HEIGHT_FULLSCREEN;
int windowWidth = WINDOW_WIDTH_SMALL, windowHeight = WINDOW_HEIGHT_SMALL;

float currentTime = 0.0f, lastTime = 0.0f;

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	WINDOW_WIDTH_FULLSCREEN = videoMode->width;
	WINDOW_HEIGHT_FULLSCREEN = videoMode->height;

	window = glfwCreateWindow(windowWidth, windowHeight, "Terrain", NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwMakeContextCurrent(window);
}

int initGL() {
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		std::cin.get();
		return 1;
	}
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glPatchParameteri(GL_PATCH_DEFAULT_OUTER_LEVEL, 5);
	glPatchParameteri(GL_PATCH_DEFAULT_INNER_LEVEL, 5);
}

void toggleFullscreen() {
	if (fullscreen) {
		windowWidth = WINDOW_WIDTH_SMALL;
		windowHeight = WINDOW_HEIGHT_SMALL;
		glfwWindowHint(GLFW_DECORATED, GL_TRUE);
	}
	else {
		windowWidth = WINDOW_WIDTH_FULLSCREEN;
		windowHeight = WINDOW_HEIGHT_FULLSCREEN;
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	}
	terrain->setProjection(windowWidth, windowHeight);
	window = glfwCreateWindow(windowWidth, windowHeight, "Terrain", NULL, NULL);
	glfwMakeContextCurrent(window);
	initGL();
}

void tick() {
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
	//if (glfwGetKey(window, GLFW_KEY_F11)) toggleFullscreen();

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
	srand(time(NULL));
	initGLFW();
	initGL();

	camera = new Camera();
	terrain = new Terrain(windowWidth, windowHeight);

	while (!glfwWindowShouldClose(window)) {
		tick();
		render();
	}
	return 0;
}