#include "sphere.h"

//--------------------------------------------------------------------------------------------
// domyslny konstruktor 
glSphere::glSphere(float R, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture) : Drawable()
{
	if (diffuseTexture != NULL)
		textures[Diffuse] = new glTexture(diffuseTexture);
	if (specularTexture != NULL)
		textures[Specular] = new glTexture(specularTexture);
	if (normalTexture != NULL)
		textures[Normal] = new glTexture(normalTexture);
	if (extraTexture != NULL)
		textures[Extra] = new glTexture(extraTexture);

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	
	// przygotuj geometrie obiektu 
	float kat = 0.0; 
	int slices = 50; 
	float dk = 360.0/float(slices-1); 
	
	float u = 0.0; 
	float du = 1.0/float(slices-1);
	
	float v = 0.0; 
	float dv = 1.0/float(slices-1);

	float phi = -90.0;
	float dphi = 180.0/float(slices-1);
	
	int cl=0;

	while (phi <= 90)
	{
		kat = 0.0;
		u = 0.0;
		while (kat <= 360.0)
		{
			vertices.push_back(glm::vec3(R*cos((phi + dphi)*PI / 180.0)*cos(kat*PI / 180.0), R*sin((phi + dphi)*PI / 180.0), R*cos((phi + dphi)*PI / 180.0)*sin(kat*PI / 180.0)));
			uvs.push_back(glm::vec2(u, v + dv));
			normals.push_back(glm::vec3(cos((phi + dphi)*PI / 180.0)*cos(kat*PI / 180.0), sin((phi + dphi)*PI / 180.0), cos((phi + dphi)*PI / 180.0)*sin(kat*PI / 180.0)));

			vertices.push_back(glm::vec3(R*cos(phi*PI / 180.0)*cos(kat*PI / 180.0), R*sin(phi*PI / 180.0), R*cos(phi*PI / 180.0)*sin(kat*PI / 180.0)));
			uvs.push_back(glm::vec2(u, v));
			normals.push_back(glm::vec3(cos(phi*PI / 180.0)*cos(kat*PI / 180.0), sin(phi*PI / 180.0), cos(phi*PI / 180.0)*sin(kat*PI / 180.0)));
	
			kat += dk;
			u+=du;
		}
		phi += dphi;
		v += dv;
	}


	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals); 

	std::reverse(indices.begin(), indices.end());
	std::reverse(indexed_vertices.begin(), indexed_vertices.end());
	std::reverse(indexed_uvs.begin(), indexed_uvs.end());
	std::reverse(indexed_normals.begin(), indexed_normals.end());
	

	vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	size = indices.size();
}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glSphere::~glSphere()
{	
}
//--------------------------------------------------------------------------------------------
void glSphere::Draw()
{
	// Bind our texture in Texture Unit 0
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(TextureID, 0);gl.activeTexture(gl.TEXTURE0);
	// 1rst attribute buffer : vertices
	glCullFace(GL_FRONT);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLE_STRIP,      // mode
		size,    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
	glCullFace(GL_BACK);
}
// the end 
