#include "object.h"
#include <glm/gtx/transform.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "scene.h"


Object::Object() {
	scale = { 1,1,1 };
	position = { 0,0,0 };
	rotation = { 0,0,0,0 };
	parentIdx = -1;
}

glm::mat4 Object::modelMatrix(Scene& currentScene) {
	glm::mat4 transMatrix = glm::translate(glm::mat4(1), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), scale);
	glm::mat4 rotMatrix = glm::toMat4(rotation);
	if (parentIdx != -1) {
		return currentScene.objects[parentIdx].modelMatrix(currentScene) * transMatrix * rotMatrix * scaleMatrix;
	} else {
		return transMatrix * rotMatrix * scaleMatrix;
	}

}
