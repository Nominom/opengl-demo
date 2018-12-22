#pragma once
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "mesh.h"

struct Scene;

struct Object {
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	std::vector<unsigned int> meshes;
	int parentIdx;

	Object();

	glm::mat4 modelMatrix(Scene &currentScene);
};