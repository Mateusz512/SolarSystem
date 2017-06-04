#include "MeshBuffers.h"


MeshBuffers::MeshBuffers()
{
}

MeshBuffers::MeshBuffers(GLuint m_VAO, GLuint size, GLuint vertexbuffer, GLuint uvbuffer, GLuint normalbuffer, GLuint elementbuffer, GLuint modelMatrixBuffer)
{
	this->m_VAO = m_VAO;
	this->size = size;
	this->vertexbuffer = vertexbuffer;
	this->uvbuffer = uvbuffer;
	this->normalbuffer = normalbuffer;
	this->elementbuffer = elementbuffer;
	this->modelMatrixBuffer = modelMatrixBuffer;
}


MeshBuffers::~MeshBuffers()
{
}
