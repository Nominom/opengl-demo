#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

GLuint CreateBuffer(std::vector<GLfloat> &data) {
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
		&data[0], GL_STATIC_DRAW);

	return bufferId;
}

Mesh::Mesh() {
	materialIdx = 0;
}

Mesh::Mesh(std::string filename) {
	materialIdx = 0;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);
	// if file not found
	if (!scene) {
		printf("Error loading object: %s", filename);
		return;
	}

	aiMesh* mesh = scene->mMeshes[0];

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

		vertexArray.push_back(vert);
	}

	//Add indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		auto face = mesh->mFaces[i];
		if (face.mNumIndices == 3) {
			elementArray.push_back(face.mIndices[0]);
			elementArray.push_back(face.mIndices[1]);
			elementArray.push_back(face.mIndices[2]);
		}
	}

}

void Mesh::GLSetup() {

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Create and bind buffer
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(Vertex),
		&vertexArray[0], GL_STATIC_DRAW);

	//Setup attrib pointers


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	//Positions
	glVertexAttribPointer(
		0, // attribute channel
		3, // x,y,z
		GL_FLOAT, // type
		GL_FALSE, // normalized
		sizeof(Vertex), // stride
		(void*)0 //offset
	);
	//uvCoord
	glVertexAttribPointer(
		1, // attribute channel
		2, // x,y
		GL_FLOAT, // type
		GL_FALSE, // normalized
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uvCoord) //offset
	);
	//Normals
	glVertexAttribPointer(
		2, // attribute channel
		3, // x,y,z
		GL_FLOAT, // type
		GL_FALSE, // normalized
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) //offset
	);
	//Tangents
	glVertexAttribPointer(
		3, // attribute channel
		3, // x,y,z
		GL_FLOAT, // type
		GL_FALSE, // normalized
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, tangent) //offset
	);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementArray.size() * sizeof(unsigned short),
		&elementArray[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

}
