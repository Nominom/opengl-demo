#include "scene.h"
#include "scene.h"
#include <glm/gtx/transform.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


void processNode(aiNode * node, const aiScene * ascene, Scene * scene, int parent = -1) {
	Object o;
	o.parentIdx = parent;
	aiVector3D scale;
	aiQuaternion rotation;
	aiVector3D position;
	node->mTransformation.Decompose(scale, rotation, position); 
	o.position = glm::vec3(position.x, position.y, position.z);
	o.scale = glm::vec3(scale.x, scale.y, scale.z);
	o.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		o.meshes.push_back(node->mMeshes[i]);
	}

	scene->objects.push_back(o);
	unsigned int thisObject = scene->objects.size() - 1;
	//process children
	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], ascene, scene, thisObject);
	}
}

Mesh processMesh(aiMesh* mesh) {
	Mesh m;

	m.materialIdx = mesh->mMaterialIndex + 1;

	//add vertex positions, uvs and normals
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vert;
		if (mesh->HasPositions()) {
			auto vPos = mesh->mVertices[i];
			vert.position.x = vPos.x;
			vert.position.y = vPos.y;
			vert.position.z = vPos.z;
		}
		if (mesh->HasNormals()) {
			auto vNorm = mesh->mNormals[i];
			vert.normal.x = vNorm.x;
			vert.normal.y = vNorm.y;
			vert.normal.z = vNorm.z;
		}
		if (mesh->HasTextureCoords(0)) {
			auto vUv = mesh->mTextureCoords[0][i];
			vert.uvCoord.x = vUv.x;
			vert.uvCoord.y = vUv.y;
		} else { // if no uv's just put 0s
			vert.uvCoord = { 0,0 };
		}

		if (mesh->HasTangentsAndBitangents()) {
			auto tang = mesh->mTangents[i];
			vert.tangent.x = tang.x;
			vert.tangent.y = tang.y;
			vert.tangent.z = tang.z;
		}

		m.vertexArray.push_back(vert);
	}

	//Add indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		auto face = mesh->mFaces[i];
		if (face.mNumIndices == 3) {
			m.elementArray.push_back(face.mIndices[0]);
			m.elementArray.push_back(face.mIndices[1]);
			m.elementArray.push_back(face.mIndices[2]);
		}
	}

	return m;
}

unsigned int loadMaterialTexture(aiMaterial *mat, aiTextureType type, Scene* scene) {
	if (mat->GetTextureCount(type) > 0) {
		aiString str;
		mat->GetTexture(type, 0, &str);

		for (unsigned int j = 0; j < scene->textures.size(); j++) {
			if (std::strcmp(scene->textures[j].path.data(), str.C_Str()) == 0) {
				printf("Found texture already loaded: %s\n", str.C_Str());
				return j;
			}
		}

		printf("Loading texture with filename: %s\n", str.C_Str());

		Texture tex(str.C_Str(), true);	
		scene->textures.push_back(tex); // add to loaded textures
		return scene->textures.size()-1;
	}
	printf("No texture found in material");
	return 0; //return white
	
}

Material processMaterial(aiMaterial* mat, Scene* scene) {
	

	unsigned int diffuse = 0;
	unsigned int normal = 1;
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		printf("Found texture! %d\n", aiTextureType_DIFFUSE);
		diffuse = loadMaterialTexture(mat, aiTextureType_DIFFUSE, scene);
	}
	if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) {
		printf("Normal map found!\n");
		printf("Found texture! %d\n", aiTextureType_NORMALS);
		normal = loadMaterialTexture(mat, aiTextureType_NORMALS, scene);
		if (normal == 0) normal = 1; //set to normal blue
	}
	

	aiColor4D diffuseCol, specularCol;
	ai_real shininess, reflectivity;
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuseCol);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specularCol);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);

	aiGetMaterialFloat(mat, AI_MATKEY_REFLECTIVITY, &reflectivity);

	printf("material diffusecol: %f, %f, %f\n", diffuseCol.r, diffuseCol.g, diffuseCol.b);
	printf("material specularCol: %f, %f, %f\n", specularCol.r, specularCol.g, specularCol.b);
	printf("material shininess: %f\n", shininess);
	printf("material reflectivity: %f\n", reflectivity);


	Material out(diffuse, normal);
	out.diffuseColor = { diffuseCol.r, diffuseCol.g, diffuseCol.b };
	out.specular = { specularCol.r, specularCol.g, specularCol.b };
	out.shininess = shininess;
	out.reflectivity = reflectivity;
	//transparent materials are unlit by default
	out.unlit = scene->textures[diffuse].hasTransparency; 

	return out;
}

int countNode(aiNode* node) {
	int number = 1;
	for (size_t i = 0; i < node->mNumChildren; i++) {
		number += countNode(node->mChildren[i]);
	}
	return number;
}

void LoadScene(Scene * scene, const std::string & filename) {
	Assimp::Importer importer;
	const aiScene* ascene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);
	
	scene->meshes.resize(ascene->mNumMeshes);
	for (int i = 0; i < ascene->mNumMeshes; i++) {
		scene->meshes[i] = processMesh(ascene->mMeshes[i]);
		scene->meshes[i].GLSetup();
	}
	scene->textures.push_back(Texture::White());
	scene->textures.push_back(Texture::NormalBlue());


	scene->materials.resize(ascene->mNumMaterials+1);
	scene->materials[0] = Material(0);
	for (int i = 0; i < ascene->mNumMaterials; i++) {
		scene->materials[i+1] = processMaterial(ascene->mMaterials[i], scene);
	}

	scene->objects.reserve(countNode(ascene->mRootNode));

	processNode(ascene->mRootNode, ascene, scene);
}
