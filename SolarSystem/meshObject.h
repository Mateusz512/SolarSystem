#pragma once

#include "common.h"
#include "vboindexer.h"
#include "texture.h"
#include <vector>
#include <sstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <filesystem>
#include "MeshManager.h"

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define MODEL_MAT_VB   4

class meshObject
{
public:
	meshObject(char * filename, 
		char * diffuseTexture, 
		char * specularTexture, 
		char * normalTexture, 
		char * extraTexture, 
		int instancesCount=0);	
	~meshObject();
	void Draw();
	float radious = 0.0f;
	
	static int meshObject::globalID;
	static MeshManager* meshObject::meshManager;
	int ID;

	meshObject() {
		ID = meshObject::globalID++;
		position = new glm::vec3(0, 0, 0);
		rotationMatrix = new glm::mat4(1);
		scale = 1.0f;
		textures = new glTexture*[4];
		for (int i = 0; i < 4; i++) {
			textures[i] = NULL;
		}
	}

	GLuint getTextureID(int texNo) {
		if (this->textures[texNo] != NULL)
			return this->textures[texNo]->GetTextureID();
		return UINT_MAX;
	}

	char* name;
	glm::mat4 * modelMatrices;
	int instancesCount=0;

	void Load(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture);
	void ReadFromFile(char * filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	void PrepareInstancesMatrices();

	GLuint m_VAO;

	glm::vec3* position;
	glm::mat4* rotationMatrix;
	float scale = 1.0f;

	glTexture** textures;
	GLuint size;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	GLuint modelMatrixBuffer;
	GLuint normalMatrixBuffer;
};

