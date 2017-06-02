#pragma once

#include "common.h"
#include "Drawable.h"
#include "vboindexer.h"
#include "texture.h"
#include <vector>
#include <sstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define MODEL_MAT_VB   4

class blendObject: public Drawable
{
public:
	blendObject();
	blendObject(char * filename, 
		char * diffuseTexture, 
		char * specularTexture, 
		char * normalTexture, 
		char * extraTexture, 
		int instancesCount=0);	
	~blendObject();
	void Draw();

protected:

	glm::mat4 * modelMatrices;
	int instancesCount;
	unsigned int modelMatricesBuffer;
	GLuint VAO;


	void Load(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture);
	void ReadFromFile(char * filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	void PrepareInstancesMatrices();
};

