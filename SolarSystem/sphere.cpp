#include "sphere.h"

//--------------------------------------------------------------------------------------------
// domyslny konstruktor 
glSphere::glSphere(float R, char* diffuseTexture, 
	char* specularTexture, char* normalTexture, 
	char* extraTexture, glm::vec3 atmoColor) 
		: blendObject("objects\\sphere.obj", 
		diffuseTexture, specularTexture, normalTexture, extraTexture)
{
	this->scale = R;
	if (atmoColor.r != 0 && atmoColor.g != 0 && atmoColor.b != 0) {
		hasAtmo = true;
		this->atmoColor = new glm::vec3(atmoColor);
	}


}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glSphere::~glSphere()
{	
}
//--------------------------------------------------------------------------------------------
void glSphere::Draw()
{
	blendObject::Draw();	
}
// the end 
