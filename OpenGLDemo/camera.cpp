#include "camera.h"
#include <glm/gtx/transform.hpp>

Camera::Camera(float aspect) {
	aspectRatio = aspect;
	position = { 0,0,0 };
	target = { 0,0,0 };
	Fov = 60;
	nearClipPlane = 0.1f;
	farClipPlane = 100;
}

glm::mat4 Camera::viewMatrix() {
	return glm::lookAt(
		position,
		target,
		{0,1,0}
	);
}

glm::mat4 Camera::projMatrix(float displayRatio) {
	return glm::perspective(glm::radians(Fov), displayRatio, nearClipPlane, farClipPlane);
}