#include "blendObject.h"

meshObject::meshObject()
{
}

meshObject::meshObject(char* filename, 
	char* diffuseTexture, 
	char* specularTexture, 
	char* normalTexture, 
	char* extraTexture, 
	int instancesCount) : Drawable()
{
	this->name = filename;
	this->instancesCount = instancesCount;
	Load(filename, diffuseTexture, specularTexture, normalTexture, extraTexture);
}


meshObject::~meshObject()
{
}

void meshObject::Draw()
{
	// Bind our texture in Texture Unit 0
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(TextureID, 0);gl.activeTexture(gl.TEXTURE0);

	// 1rst attribute buffer : vertices

	// 2nd attribute buffer : UVs

	// 3rd attribute buffer : normals

	// Index buffer
	glBindVertexArray(m_VAO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	if (instancesCount > 0) {
		glDrawElementsInstanced(GL_TRIANGLES,
			size,
			GL_UNSIGNED_SHORT,
			(void*)(0),
			instancesCount);
	}
	else {
		glDrawElements(
			GL_TRIANGLES,      // mode
			size,    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);		
	}
	glBindVertexArray(0);
}




void meshObject::Load(char* filename, char* diffuseTexture, char* specularTexture, char* normalTexture, char* extraTexture) {

	if (diffuseTexture != NULL && std::experimental::filesystem::exists(diffuseTexture))
		textures[Diffuse] = new glTexture(diffuseTexture);
	if (specularTexture != NULL && std::experimental::filesystem::exists(specularTexture))
		textures[Specular] = new glTexture(specularTexture);
	if (normalTexture != NULL && std::experimental::filesystem::exists(normalTexture))
		textures[Normal] = new glTexture(normalTexture);
	if (extraTexture != NULL && std::experimental::filesystem::exists(extraTexture))
		textures[Extra] = new glTexture(extraTexture);
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	
	MeshBuffers* meshBuffers = meshManager->alreadyLoaded(filename);
	if (meshBuffers != NULL && instancesCount==0) {
		this->m_VAO = meshBuffers->m_VAO;
		this->size = meshBuffers->size;
		this->vertexbuffer = meshBuffers->vertexbuffer;
		this->uvbuffer = meshBuffers->uvbuffer;
		this->normalbuffer = meshBuffers->normalbuffer;
		this->elementbuffer = meshBuffers->elementbuffer;
		this->modelMatrixBuffer = meshBuffers->modelMatrixBuffer;
	}
	else {

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		ReadFromFile(filename, vertices, uvs, normals);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

		vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		normalbuffer;
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Generate a buffer for the indices as well
		elementbuffer;
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

		if (instancesCount > 0) {
			PrepareInstancesMatrices();

			glGenBuffers(1, &modelMatrixBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBuffer);

			for (unsigned int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(3 + i);
				glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
					(const GLvoid*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor(3 + i, 1);
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instancesCount, modelMatrices, GL_DYNAMIC_DRAW);


			for (int i = 0; i < instancesCount; i++)
			{
				modelMatrices[i] = glm::transpose(glm::inverse(modelMatrices[i]));
			}

			glGenBuffers(1, &normalMatrixBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, normalMatrixBuffer);

			for (unsigned int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(7 + i);
				glVertexAttribPointer(7 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
					(const GLvoid*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor(7 + i, 1);
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instancesCount, modelMatrices, GL_DYNAMIC_DRAW);
			delete[] modelMatrices;
		}

		glBindVertexArray(0);
		size = indices.size();

		meshManager->add(filename,new MeshBuffers(m_VAO,size, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, modelMatrixBuffer));
	}
}

void meshObject::ReadFromFile(char * filename,
	std::vector < glm::vec3 > & out_vertices,
	std::vector < glm::vec2 > & out_uvs,
	std::vector < glm::vec3 > & out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		std::stringstream sstr;
		sstr << "Cannot open object file '" << filename <<"'";
		std::string str = sstr.str();
		ThrowException(&str);
	}
	glm::vec2 max = glm::vec2(0, 0);
	glm::vec2 cur = glm::vec2(0, 0);

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				ThrowException("Parser error!");
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			cur.x = vertexIndex[0];
			cur.y = vertexIndex[2];
			if (glm::length(max) > glm::length(cur)) {
				max.x = cur.x;
				max.y = cur.y;
			}
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int index = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[index - 1];
		out_vertices.push_back(vertex);

		index = uvIndices[i];
		glm::vec2 uv = temp_uvs[index - 1];
		out_uvs.push_back(uv);

		index = normalIndices[i];
		glm::vec3 normal = temp_normals[index - 1];
		out_normals.push_back(normal);
	}
	radious = glm::length(max);
}

void meshObject::PrepareInstancesMatrices() {
	modelMatrices = new glm::mat4[instancesCount];
	srand(1337); // initialize random seed	
	float radius = 25.0;
	float offset = 5.0f;
	for (int i = 0; i < instancesCount; i++)
	{
		glm::mat4 model;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)instancesCount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.1f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale/2.0f));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
}