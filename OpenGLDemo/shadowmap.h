#pragma once
#include "light.h"
#include "shader.hpp"
#include "scene.h"

struct ShadowMap {
	GLuint shadowTextureID;
	GLuint FBO;
	Light* light;
	unsigned int width, height;
	float viewWidth;
	float viewHeight;
	float nearPlane;
	float farPlane;

	ShadowMap(Light *l, unsigned int w, unsigned int h, float viewWidth, float viewHeight);

	void RenderShadowMap(Scene &scene, Shader &shader, Camera &cam, int windowWidth, int windowHeight);
	glm::mat4 lightSpaceMatrix(Camera &cam);
};