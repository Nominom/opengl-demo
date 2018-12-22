#pragma once
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <vector>
#include "camera.h"
#include "shader.hpp"

struct Skybox{
	GLuint textureID;
	GLuint VAO;
	GLuint VBO;
	int width;
	int height;
	int colorChannels;

	Skybox(const std::string &right, const std::string &left, const std::string &top,
		const std::string &bottom, const std::string &front, const std::string &back);
	Skybox(const std::vector<std::string> &faces); //right, left, top, bottom, front, back

	void Draw(const Shader &skyboxShader, Camera &camera);

private:
	void Setup(const std::vector<std::string> &faces);
};