#include <stdio.h>
#include <stdlib.h>
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include "shader.hpp"
#include "Window.h"
#include "object.h"
#include "camera.h"
#include "texture.h"
#include "light.h"
#include "scene.h"
#include "skybox.h"
#include "shadowmap.h"


const int width = 1024;
const int height = 760;

void drawScene(Scene& scene, Shader &shader, Camera &cam, Light &light, Skybox &skybox, ShadowMap &shadowMap) {
	glUseProgram(shader.programID);
	shader.SetCamLightVariables(cam, light, width / height);
	shader.SetLightSpaceMatrix(shadowMap.lightSpaceMatrix(cam));

	//shadowMap
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowMap.shadowTextureID);

	for (int i = 0; i < scene.objects.size(); i++) {
		Object &obj = scene.objects[i];

		shader.SetObjVariables(scene, obj);

		for (int j = 0; j < obj.meshes.size(); j++) {
			Mesh &mesh = scene.meshes[obj.meshes[j]];
			Material &mat = scene.materials[mesh.materialIdx];
			Texture &diffuse = scene.textures[mat.diffuseMap];
			Texture &normalMap = scene.textures[mat.normalMap];

			//diffuse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuse.textureId);

			//normal
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap.textureId);

			
			//skybox
			if (mat.reflectivity > 0) {
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
			}

			glBindVertexArray(mesh.vertexArrayObject);

			shader.SetMeshMaterialVariables(mat);

			glDrawElements(
				GL_TRIANGLES,
				mesh.elementArray.size(),
				GL_UNSIGNED_SHORT,
				(void*)0
			);
		}
	}
}


int main(void) {
	auto window = InitWindow(width, height, "OpenGL demo");

	if (window == NULL) {
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);


	Scene scene;
	LoadScene(&scene, "scene.dae");


	Light light;
	light.direction = { 1, -1, -1 };
	light.color = { 1, 1, 1 };


	Camera camera(width/height);

	camera.position = {-3, 1,-3 };
	camera.target = { 0, 0, 0 };

	Shader standardShader("shaders/VertexShader.glsl", "shaders/DefaultFragment.glsl", false);
	Shader skyboxShader("shaders/SkyboxVertexShader.glsl", "shaders/SkyboxFragmentShader.glsl", true);
	Shader shadowShader("shaders/ShadowVertexShader.glsl", "shaders/ShadowFragmentShader.glsl", true);

	Skybox skybox("skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg");

	ShadowMap shadows(&light, 2048, 2048, 10, 10);

	//glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);


	double oldTimeSinceStart = glfwGetTime();
	double lastFrameCheck = oldTimeSinceStart;
	int frames = 0;

	do {
		frames++;
		double timeSinceStart = glfwGetTime();
		double deltaTime = timeSinceStart - oldTimeSinceStart;
		oldTimeSinceStart = timeSinceStart;

		if (timeSinceStart > lastFrameCheck + 1) {
			lastFrameCheck = timeSinceStart;
			printf("FPS: %d\n", frames);
			frames = 0;
		}

		camera.position = { 
			-3 + sin(timeSinceStart),
			1,
			-3 + cos(timeSinceStart)};

		shadows.RenderShadowMap(scene, shadowShader, camera, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.Draw(skyboxShader, camera);

		drawScene(scene, standardShader, camera, light, skybox, shadows);



		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}