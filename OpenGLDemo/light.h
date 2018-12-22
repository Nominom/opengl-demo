#pragma once
#include <glm/gtx/transform.hpp>
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"

class Light {
public:
	glm::vec3 color;
	glm::vec3 direction;
	Light();
};