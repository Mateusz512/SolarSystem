#pragma once

#include "common.h"
#include "Drawable.h"
#include "vboindexer.h"
#include "texture.h"
#include <vector>
#include <sstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <filesystem>

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define MODEL_MAT_VB   4

class meshObject: public Drawable
{
public:
	meshObject();
	meshObject(char * filename, 
		char * diffuseTexture, 
		char * specularTexture, 
		char * normalTexture, 
		char * extraTexture, 
		int instancesCount=0);	
	~meshObject();
	void Draw();
	float radious = 0.0f;

protected:
	char* name;
	glm::mat4 * modelMatrices;
	int instancesCount=0;

	void Load(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture);
	void ReadFromFile(char * filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	void PrepareInstancesMatrices();
};

