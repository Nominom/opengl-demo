#include "shadowmap.h"

ShadowMap::ShadowMap(Light * l, unsigned int w, unsigned int h, float vWidth, float vHeight) {
	nearPlane = 1.0f;
	farPlane = 10;
	width = w;
	height = h;
	light = l;
	viewWidth = vWidth;
	viewHeight = vHeight;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowTextureID);
	glBindTexture(GL_TEXTURE_2D, shadowTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::RenderShadowMap(Scene & scene, Shader & shader, Camera &cam, int windowWidth, int windowHeight) {
	// Render depth map
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lsm = lightSpaceMatrix(cam);

	glUseProgram(shader.programID);
	shader.SetLightSpaceMatrix(lsm);

	//Render objects in scene
	for (int i = 0; i < scene.objects.size(); i++) {
		Object &obj = scene.objects[i];

		shader.SetObjVariables(scene, obj);

		for (int j = 0; j < obj.meshes.size(); j++) {
			Mesh &mesh = scene.meshes[obj.meshes[j]];
			Material &mat = scene.materials[mesh.materialIdx];
			Texture &diffuse = scene.textures[mat.diffuseMap];

			//diffuse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuse.textureId);

			glBindVertexArray(mesh.vertexArrayObject);

			glDrawElements(
				GL_TRIANGLES,
				mesh.elementArray.size(),
				GL_UNSIGNED_SHORT,
				(void*)0
			);
		}
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Set normal values and clear the framebuffers
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::mat4 ShadowMap::lightSpaceMatrix(Camera & cam) {
	glm::vec3 viewDir = glm::normalize(cam.target - cam.position);
	glm::vec3 shadowTarget = cam.position + (viewDir * (viewWidth * 0.5f));


	glm::mat4 lightProjection = glm::ortho(-viewWidth, viewWidth, -viewHeight, viewHeight, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(
		shadowTarget - (light->direction * (farPlane / 3)),
		shadowTarget,
		glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}
