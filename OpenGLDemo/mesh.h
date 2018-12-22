#pragma once
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec2 uvCoord;
	glm::vec3 normal;
	glm::vec3 tangent;
};

struct Mesh {
	std::vector<Vertex> vertexArray;
	std::vector<unsigned short> elementArray;
	GLuint vertexArrayObject;
	GLuint vertexBufferObject;
	GLuint elementBuffer;
	unsigned int materialIdx;

	Mesh();
	Mesh(std::string filename);

	void GLSetup();
};