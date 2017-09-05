#include "meshObject.h"


int meshObject::staticID = 1;
AlreadyLoadedHelper* meshObject::alreadyLoadedHelper = NULL;

meshObject::meshObject(char* filename, 
	char* diffuseTexture, 
	char* specularTexture, 
	char* normalTexture, 
	char* extraTexture) : meshObject()
{
	this->name = filename;
	Load(filename, diffuseTexture, specularTexture, normalTexture, extraTexture);
}

meshObject::~meshObject()
{
}

void meshObject::Draw()
{
	// Index buffer
	glBindVertexArray(this->meshBuffers->m_VAO);
	
	glDrawElements(
		GL_TRIANGLES,      // mode
		this->meshBuffers->size,    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);		

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

	this->meshBuffers = alreadyLoadedHelper->alreadyLoaded(filename);
	if (meshBuffers == NULL) {
		this->meshBuffers = new MeshBuffers();
		glGenVertexArrays(1, &(this->meshBuffers->m_VAO));
		glBindVertexArray(this->meshBuffers->m_VAO);

		std::clock_t begin = clock();

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;

		SuperCoolRead(JoinTwoStrings(filename, ".elo"), indices, indexed_vertices, indexed_uvs, indexed_normals, radious);
		if (indices.size() == 0) {
			std::vector< glm::vec3 > vertices;
			std::vector< glm::vec2 > uvs;
			std::vector< glm::vec3 > normals;
			ReadFromFile(JoinTwoStrings(filename, ".obj"), vertices, uvs, normals);

			indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
			SuperCoolWrite(JoinTwoStrings(filename, ".elo"), indices, indexed_vertices, indexed_uvs, indexed_normals, radious);
		}

		std::clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		this->meshBuffers->vertexbuffer;
		glGenBuffers(1, &(this->meshBuffers->vertexbuffer));
		glBindBuffer(GL_ARRAY_BUFFER, this->meshBuffers->vertexbuffer);
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

		this->meshBuffers->uvbuffer;
		glGenBuffers(1, &(this->meshBuffers->uvbuffer));
		glBindBuffer(GL_ARRAY_BUFFER, this->meshBuffers->uvbuffer);
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

		this->meshBuffers->normalbuffer;
		glGenBuffers(1, &(this->meshBuffers->normalbuffer));
		glBindBuffer(GL_ARRAY_BUFFER, this->meshBuffers->normalbuffer);
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
		this->meshBuffers->elementbuffer;
		glGenBuffers(1, &(this->meshBuffers->elementbuffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshBuffers->elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		this->meshBuffers->size = indices.size();

		alreadyLoadedHelper->add(filename, this->meshBuffers);
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

void meshObject::SuperCoolRead(const char * path, std::vector<unsigned short> & out_indices, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, float& radious)
{
	if(!std::experimental::filesystem::exists(path)) return;
	std::ifstream ifs(path, std::ios_base::in | std::ios_base::binary);
	if (ifs)
	{
		ifs.unsetf(std::ios::skipws);
		ifs.seekg(0, std::ios::beg);
		int size = 0;
		int i;
		ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (i = 0; i < size; i++) {
			unsigned short temp;
			ifs.read(reinterpret_cast<char*>(&temp), 1 * sizeof(unsigned short));
			out_indices.push_back(temp);
		}
		ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (i = 0; i < size; i++) {
			glm::vec3 temp;
			ifs.read(reinterpret_cast<char*>(&temp), 1 * sizeof(glm::vec3));
			out_vertices.push_back(temp);
		}
		ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (i = 0; i < size; i++) {
			glm::vec2 temp;
			ifs.read(reinterpret_cast<char*>(&temp), 1 * sizeof(glm::vec2));
			out_uvs.push_back(temp);
		}
		ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (i = 0; i < size; i++) {
			glm::vec3 temp;
			ifs.read(reinterpret_cast<char*>(&temp), 1 * sizeof(glm::vec3));
			out_normals.push_back(temp);
		}
		ifs.read(reinterpret_cast<char*>(&radious), sizeof(float));
		ifs.close();
	}
}

void meshObject::SuperCoolWrite(const char * path, std::vector<unsigned short> & out_indices, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, float& radious)
{
	std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);
	if (ofs)
	{
		int size = out_indices.size();
		ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<char*>(&out_indices[0]), size * sizeof(out_indices[0]));
		size = out_vertices.size();
		ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<char*>(&out_vertices[0]), size * sizeof(out_vertices[0]));
		size = out_uvs.size();
		ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<char*>(&out_uvs[0]), size * sizeof(out_uvs[0]));
		size = out_normals.size();
		ofs.write(reinterpret_cast<char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<char*>(&out_normals[0]), size * sizeof(out_normals[0]));
		ofs.write(reinterpret_cast<char*>(&radious), 1 * sizeof(float));
		ofs.close();
	}
}

