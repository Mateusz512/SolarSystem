#ifndef sphere_H
#define sphere_H

#include "blendObject.h"
#include "texture.h"
#include "common.h"

class glSphere: public Drawable
{
	public: 
		glSphere(float R, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture); // domyslny konstruktor 
		~glSphere(); // domyslny destrutor 
		void Draw();
	protected:	
};

#endif
