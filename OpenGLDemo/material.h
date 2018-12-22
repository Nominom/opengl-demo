#pragma once
#include "texture.h"
#include <glm/glm.hpp>

struct Material {
	unsigned int diffuseMap; //Index to scene
	unsigned int normalMap; //Index to scene;
	glm::vec3 diffuseColor;
	glm::vec3 specular;
	float shininess;
	float reflectivity;
	bool unlit;

	Material();
	Material(unsigned int diffuse);
	Material(unsigned int diffuse, unsigned int normal);
};