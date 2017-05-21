#pragma once

#include "common.h"
#include "Drawable.h"
#include "vboindexer.h"
#include "texture.h"
#include <vector>
#include <sstream>
#include "glm\glm.hpp"


class blendObject: public Drawable
{
public:
	blendObject();
	blendObject(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture);
	~blendObject();
	void Draw();

protected:

	void Load(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture);
	void ReadFromFile(char * filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
};

