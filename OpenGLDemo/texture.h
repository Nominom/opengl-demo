#pragma once
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>

class Texture {
public:
	GLuint textureId;
	int width;
	int height;
	int colorChannels;
	bool hasTransparency;
	std::string path;

	Texture(std::string filepath, bool flip);

	static Texture White();
	static Texture NormalBlue();
private:
	Texture();
};