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
#include "FastNoise.h"

#define TEXTURE_WIDTH 500
#define TEXTURE_HEIGHT 500

class Terrain
{
public:
	Terrain(int windowWidth, int windowHeight);

	void setProjection(int windowWidth, int windowHeight);
	void render(Camera* camera);

	~Terrain();
private:
	typedef struct chunk {
		GLuint textureID;
		glm::mat4 transform;
	} Chunk;

	typedef struct rgb {
		unsigned char r, g, b, a;
	} RGB;

	GLuint vao;
	GLuint programID;
	GLuint eyeUniform, projectionUniform, heightMapUniform, transformUniform;

	std::vector<Chunk*> chunks;

	glm::mat4 projection;

	FastNoise* noise;

	void createMesh();
	GLuint createTexture(int x, int z);
	void createShader();
	void setupUniforms();

	void addChunk(int x, int z);

	static std::string readFile(std::string path) {
		std::ifstream f;
		std::string line;
		std::string content;
		f.open("res/" + path, std::ios::in);
		if (f.is_open()) {
			while (std::getline(f, line)) {
				content.append(line + '\n');
			}
			f.close();
		}
		return content;
	}
};

