#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 target;
	float Fov;
	float nearClipPlane;
	float farClipPlane;
	float aspectRatio;

	Camera(float aspect);
	glm::mat4 viewMatrix();
	glm::mat4 projMatrix(float displayRatio);
};