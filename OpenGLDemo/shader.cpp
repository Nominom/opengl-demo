#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GLEW/GL/glew.h>

#include "shader.hpp"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	} else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

Shader::Shader(const char * vertex_file_path, const char * fragment_file_path, bool unlit) {

	programID = LoadShaders(vertex_file_path, fragment_file_path);
	this->unlit = unlit;

	modelMatID = glGetUniformLocation(programID, "model");
	viewMatID = glGetUniformLocation(programID, "view");
	projMatID = glGetUniformLocation(programID, "projection");
	normalMatID = glGetUniformLocation(programID, "normalMat");
	lsMatID = glGetUniformLocation(programID, "lightSpaceMatrix");

	//Lighting
	if (!unlit) {
		lightDirID = glGetUniformLocation(programID, "light.direction");
		lightColID = glGetUniformLocation(programID, "light.color");
		camPosID = glGetUniformLocation(programID, "cameraPos");
	}
	//Material
	colorID = glGetUniformLocation(programID, "material.color");
	specularID = glGetUniformLocation(programID, "material.specular");
	shininessID = glGetUniformLocation(programID, "material.shininess");
	reflectivityID = glGetUniformLocation(programID, "material.reflectivity");
	unlitID = glGetUniformLocation(programID, "material.unlit");

	glUseProgram(programID);


	GLuint diffuseID = glGetUniformLocation(programID, "material.diffuse");
	glUniform1i(diffuseID, 0);

	GLuint normalID = glGetUniformLocation(programID, "material.normalmap");
	glUniform1i(normalID, 1);

	GLuint skyboxID = glGetUniformLocation(programID, "skybox");
	glUniform1i(skyboxID, 2);

	GLuint shadowMapID = glGetUniformLocation(programID, "shadowMap");
	glUniform1i(shadowMapID, 3);

}
void Shader::SetLightSpaceMatrix(glm::mat4 lightSpaceMatrix) {
	glUniformMatrix4fv(lsMatID, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}

void Shader::SetCamLightVariables(Camera & camera, Light & light, float aspectRatio) {
	glUniformMatrix4fv(viewMatID, 1, GL_FALSE, &camera.viewMatrix()[0][0]);
	glUniformMatrix4fv(projMatID, 1, GL_FALSE, &camera.projMatrix(aspectRatio)[0][0]);

	if (!unlit) {
		glm::vec3 ldNorm = glm::normalize(light.direction);
		glm::vec3 lcolor = light.color; 
		glUniform3f(lightDirID, ldNorm.x, ldNorm.y, ldNorm.z);
		glUniform3f(lightColID, lcolor.x, lcolor.y, lcolor.z);
		glUniform3f(camPosID, camera.position.x, camera.position.y, camera.position.z);
	}
}

void Shader::SetObjVariables(Scene &scene, Object &object) {
	glm::mat4 modelMat = object.modelMatrix(scene);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &modelMat[0][0]);

	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
	glUniformMatrix3fv(normalMatID, 1, GL_FALSE, &normalMat[0][0]);
}

void Shader::SetMeshMaterialVariables(Material& mat) {
	glm::vec3 spec = mat.specular;
	glm::vec3 col = mat.diffuseColor;
	glUniform3f(specularID, spec.r, spec.g, spec.b);
	glUniform3f(colorID, col.r, col.g, col.b);
	glUniform1f(shininessID, mat.shininess);
	glUniform1f(reflectivityID, mat.reflectivity);
	glUniform1i(unlitID, mat.unlit);
}