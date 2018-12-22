#ifndef SHADER_HPP
#define SHADER_HPP
#include "object.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "material.h"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

struct Shader {
	GLuint programID;
	bool unlit;
	Shader(const char * vertex_file_path, const char * fragment_file_path, bool unlit);
	void SetLightSpaceMatrix(glm::mat4 lightSpaceMatrix);
	void SetCamLightVariables(Camera & camera, Light & light, float aspectRatio);
	void SetObjVariables(Scene &scene, Object &object);
	void SetMeshMaterialVariables(Material& mat);
	//void SetVariablesInstanced(InstancedObject &object, Camera &cam, Light &light, float aspectRatio);

	//~Shader();

	GLuint modelMatID;
	GLuint viewMatID;
	GLuint projMatID;
	GLuint normalMatID;
	GLuint lsMatID;
	//Lighting
	GLuint lightDirID;
	GLuint lightColID;
	GLuint camPosID;
	//Material
	GLuint colorID;
	GLuint specularID;
	GLuint shininessID;
	GLuint reflectivityID;
	GLuint unlitID;
};

#endif
