#pragma once
#include "shader.h"
#include "meshObject.h"
#include <glm/glm.hpp>

class Skybox

{
public:
	Skybox(Shader* shader);
	~Skybox();
	void Draw(glm::mat4 &projection, glm::mat4 &view);
private:
	GLuint skyboxVAO;
	GLuint cubemapTexture;
	Shader* shader;
	GLuint loadCubeMap(std::vector<const GLchar*> faces); 
	
};

