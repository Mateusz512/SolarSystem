#pragma once

#include "common.h"
#include "vboindexer.h"
#include "texture.h"
#include <vector>
#include <sstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <filesystem>
#include "AlreadyLoadedHelper.h"

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
		char * extraTexture);	
	~meshObject();
	void Draw();
	float radious = 0.0f;
	
	static int meshObject::staticID;
	static AlreadyLoadedHelper* meshObject::alreadyLoadedHelper;
	int ID;

	meshObject() {
		ID = meshObject::staticID++;
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

	void Load(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture);
	void ReadFromFile(char * filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);

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

