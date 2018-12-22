#include <stdio.h>
#include <stdlib.h>
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include "Window.h"

GLFWwindow* InitWindow(int width, int height, std::string windowName) {
	if (!glfwInit()) {
		fprintf(stderr, "GLFW error");
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(
		width,
		height,
		windowName.c_str(),
		NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Window failed.");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW");
		getchar();
		glfwTerminate();
		return NULL;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return window;
}