#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLEW/glew.h>

#include "Camera.h"
#include "Terrain.h"
#include "PerlinNoise.h"

class Terrain
{
public:
	Terrain(float windowWidth, float windowHeight);

	void render(Camera* camera);

	~Terrain();
private:
	GLuint vao;
	GLuint programID;
	GLuint eyeUniform, projectionUniform, heightMapUniform;
	GLuint textureID;

	glm::mat4 projection;

	void createMesh();
	void createShader();
	void setupUniforms();
	void createTexture();

	static std::string readFile(std::string path) {
		std::ifstream f;
		std::string line;
		std::string content;
		f.open(path, std::ios::in);
		if (f.is_open()) {
			while (std::getline(f, line)) {
				content.append(line + '\n');
			}
			f.close();
		}
		return content;
	}
};

