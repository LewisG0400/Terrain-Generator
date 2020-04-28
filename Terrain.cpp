#include "Terrain.h"

Terrain::Terrain(float windowWidth, float windowHeight) {
	createMesh();
	createShader();
	setupUniforms();
	createTexture();

	projection = glm::perspective(45.0f, windowWidth / windowHeight, 0.01f, 100.0f);
}

void Terrain::createMesh() {
	GLuint vbo, tbo, ibo;

	GLfloat vertices[] = {
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f
	};

	for (int i = 0; i < 12; i++) {
		vertices[i] *= 2.0f;
	}

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
}

void Terrain::createTexture() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int width = 1500, height = 1500;

	double frequency = 14.0;
	double fx = width / frequency, fy = height / frequency;
	int octaves = 5;

	int seed = 25536885;

	std::vector<unsigned char> data;
	//unsigned char* data;
	//data = new unsigned char[width * height * 3];//(unsigned char*)malloc(width * height * 3 * sizeof(char));

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			//data[x * 3 + y * width] = (unsigned char)255;// (double)x / width * 255.0;
			//data[x * 3 + (y * width) + 1] = (unsigned char)0;//(double)x / width * 255.0;
			//data[x * 3 + (y * width) + 2] = (unsigned char)0;// (double)x / width * 255.0;
			double value = perlinNoise2D(x, y, 0.6, 8) * 127.5 + 127.5;
			//std::cout << value << std::endl;
			data.push_back(value);
			data.push_back(value);
			data.push_back(value);
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Terrain::render(Camera* camera) {
	glUseProgram(programID);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glUniformMatrix4fv(eyeUniform, 1, GL_FALSE, glm::value_ptr(camera->getTransform()));
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
}

Terrain::~Terrain() {
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vao);
}