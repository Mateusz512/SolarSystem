#pragma once
#include "glew.h"

class MeshBuffers
{
public:
	MeshBuffers();
	MeshBuffers(
		GLuint m_VAO,
		GLuint size,
		GLuint vertexbuffer,
		GLuint uvbuffer,
		GLuint normalbuffer,
		GLuint elementbuffer,
		GLuint modelMatrixBuffer);
	~MeshBuffers();

	GLuint m_VAO;
	GLuint size;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	GLuint modelMatrixBuffer;
};

