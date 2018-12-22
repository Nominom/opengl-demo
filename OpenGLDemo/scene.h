#pragma once
#include "material.h"
#include "light.h"
#include "camera.h"
#include <string>
#include "object.h"


struct Scene {
	std::vector<Object> objects;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Texture> textures;
};

void LoadScene(Scene *scene, const std::string &filename);