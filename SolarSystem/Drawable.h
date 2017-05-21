#pragma once
#include <glm\glm.hpp>
#include "texture.h"

class Drawable {
public:
	Drawable() {
		position = new glm::vec3(0, 0, 0);
		rotationMatrix = new glm::mat4(1);
		scale = 1.0f;
		textures = new glTexture*[4];
		for (int i = 0; i < 4; i++) {
			textures[i] = NULL;
		}
	}
	virtual void Draw() = 0;
	GLuint getTextureID(int texNo) {
		if(this->textures[texNo] != NULL)
			return this->textures[texNo]->GetTextureID();
		return UINT_MAX;
	}


	glm::vec3* position;
	glm::mat4* rotationMatrix;
	float scale = 1.0f;

	void setPos(glm::vec3 & pos)
	{
		this->position = new glm::vec3(pos);
	}

	void setRotationMatrix(glm::mat4 & matrix)
	{
		this->rotationMatrix = new glm::mat4(matrix);
	}
protected:

	glTexture** textures;
	GLuint size;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
};