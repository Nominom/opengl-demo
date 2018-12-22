#include "material.h"

Material::Material(unsigned int diffuse, unsigned int normal) {
	diffuseMap = diffuse;
	normalMap = normal;
	diffuseColor = { 1, 1, 1 };
	specular = { 0.3, 0.3, 0.3 };
	shininess = 4;
	reflectivity = 0;
	unlit = false;
}

Material::Material(unsigned int diffuse) {
	diffuseMap = diffuse;
	normalMap = 1;
	diffuseColor = { 1, 1, 1 };
	specular = { 0.3, 0.3, 0.3 };
	shininess = 4;
	reflectivity = 0;
	unlit = false;
}

Material::Material() {
	diffuseMap = 0;
	normalMap = 1;
	diffuseColor = { 1, 1, 1 };
	specular = { 0.3, 0.3, 0.3 };
	shininess = 4;
	reflectivity = 0;
	unlit = false;
}
