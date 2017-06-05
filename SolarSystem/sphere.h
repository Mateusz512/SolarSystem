#ifndef sphere_H
#define sphere_H

#include "blendObject.h"
#include "texture.h"
#include "common.h"

class glSphere: public blendObject
{
	public: 
		glSphere(float R, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture, glm::vec3 atmoColor); // domyslny konstruktor 
		~glSphere(); // domyslny destrutor 
		
		void Draw();

		bool hasAtmo;
		glm::mat4** atmoRot;
		int atmoCount = 2;

		glm::vec3* atmoColor;
	protected:	
};

#endif
