#include "Terrain.h"

Terrain::Terrain(int windowWidth, int windowHeight) {
	noise = new FastNoise();
	noise->SetNoiseType(FastNoise::SimplexFractal);
	noise->SetSeed(rand());
	noise->SetFrequency(TEXTURE_WIDTH * 0.000004);
	noise->SetFractalOctaves(4);

	addChunk(0, 0);
	addChunk(1, 0);
	addChunk(2, 0);
	addChunk(0, 1);
	addChunk(1, 1);
	addChunk(2, 1);

	setProjection(windowWidth, windowHeight);
}

void Terrain::setProjection(int windowWidth, int windowHeight) {
	createMesh();
	createShader();
	setupUniforms();

	std::cout << windowWidth << ", " << windowHeight << std::endl;
	projection = glm::perspective(45.0f, (float)(windowWidth) / (float)(windowHeight), 0.01f, 100.0f);
	glUseProgram(programID);
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
}

void Terrain::createShader() {
	GLuint vertID, fragID, geometryID, tessControlID, tessEvalID;

	std::string vertSourceString = readFile("shader.vert");
	const char* vertSource = vertSourceString.c_str();
	std::string fragSourceString = readFile("shader.frag");
	const char* fragSource = fragSourceString.c_str();
	std::string geomSourceString = readFile("terrain.geometry");
	const char* geomSource = geomSourceString.c_str();
	std::string tessControlString = readFile("terrain.tc");
	const char* tessControlSource = tessControlString.c_str();
	std::string tessEvalString = readFile("terrain.te");
	const char* tessEvalSource = tessEvalString.c_str();

	GLint success;
	GLchar infoLog[512];

	vertID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertID, 1, &vertSource, NULL);
	glCompileShader(vertID);
	glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragID, 1, &fragSource, NULL);
	glCompileShader(fragID);
	glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	geometryID = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryID, 1, &geomSource, NULL);
	glCompileShader(geometryID);
	glGetShaderiv(geometryID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometryID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	tessControlID = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tessControlID, 1, &tessControlSource, NULL);
	glCompileShader(tessControlID);
	glGetShaderiv(tessControlID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(tessControlID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::TESSELLATION_CONTROL::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	tessEvalID = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tessEvalID, 1, &tessEvalSource, NULL);
	glCompileShader(tessEvalID);
	glGetShaderiv(tessEvalID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(tessEvalID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::TESSELLATION_EVALUATION::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertID);
	glAttachShader(programID, fragID);
	glAttachShader(programID, tessEvalID);
	glAttachShader(programID, geometryID);
	glAttachShader(programID, tessControlID);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	std::cout << vertSource << std::endl;

	glDeleteShader(vertID);
	glDeleteShader(fragID);
	glDeleteShader(geometryID);
	glDeleteShader(tessControlID);
	glDeleteShader(tessEvalID);
}

void Terrain::setupUniforms() {
	eyeUniform = glGetUniformLocation(programID, "eye");
	projectionUniform = glGetUniformLocation(programID, "projection");
	heightMapUniform = glGetUniformLocation(programID, "heightMap");
	transformUniform = glGetUniformLocation(programID, "transform");
}

void Terrain::createMesh() {
	GLuint vbo, tbo, ibo;

	GLfloat vertices[] = {
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f
	};

	GLfloat texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f };

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glCreateBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glCreateBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

GLuint Terrain::createTexture(int xOffset, int zOffset) {
	GLuint textureID;

	//unsigned char* data = new unsigned char[TEXTURE_WIDTH * TEXTURE_HEIGHT * 3];
	RGB* data = new RGB[TEXTURE_WIDTH * TEXTURE_HEIGHT];

	for (int z = 0; z < TEXTURE_HEIGHT; z++) {
		for (int x = 0; x < TEXTURE_WIDTH; x++) {
			double value = noise->GetNoise(x + xOffset * TEXTURE_WIDTH, z + zOffset * TEXTURE_HEIGHT);
			value *= 127.5;
			value -= 127.5;

			RGB colour = RGB();
			colour.r = value;
			colour.g = value;
			colour.b = value;
			colour.a = 0;

			int index = x + z * TEXTURE_WIDTH;

			data[index] = colour;
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] data;

	return textureID;
}

void Terrain::addChunk(int x, int z) {
	Chunk* chunk = new Chunk();
	chunk->textureID = createTexture(x, z);
	chunk->transform = glm::translate(glm::mat4(1.0f), glm::vec3(x * 2, 0.0, z * 2));

	chunks.push_back(chunk);
}

void Terrain::render(Camera* camera) {
	glUseProgram(programID);
	glBindVertexArray(vao);

	for (auto chunk : chunks) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, chunk->textureID);

		glUniformMatrix4fv(eyeUniform, 1, GL_FALSE, glm::value_ptr(camera->getTransform()));
		glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(chunk->transform));

		glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
	}
}

Terrain::~Terrain() {
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vao);
	
	for (auto chunk : chunks) {
		glDeleteTextures(1, &chunk->textureID);
		delete chunk;
	}

	delete noise;
}