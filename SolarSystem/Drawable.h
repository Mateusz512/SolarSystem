#pragma once
#include <glm\glm.hpp>
#include "texture.h"
#include "MeshManager.h"

class Drawable {
public:
	static int Drawable::globalID;
	static MeshManager* Drawable::meshManager;
	int ID;
	Drawable() {
		ID = Drawable::globalID++;
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

	Drawable* parent;
	glm::vec3* position;
	float orbitSpeed;
	float rotSpeed;
	glm::mat4* rotationMatrix;
	glm::vec3* inclination;			/// TODO: Implement inclination
	float scale = 1.0f;
		
	void setOrbitInfo(Drawable* parent, glm::vec3 pos, float orbitSpeed, float rotSpeed) {
		this->parent = parent;
		this->orbitSpeed = orbitSpeed;
		this->rotSpeed = rotSpeed; 
		this->position = new glm::vec3(pos);
	}
protected:
	GLuint m_VAO;

	glTexture** textures;
	GLuint size;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	GLuint modelMatrixBuffer;
	GLuint normalMatrixBuffer;
};