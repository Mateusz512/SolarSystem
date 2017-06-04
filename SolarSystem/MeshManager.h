#pragma once
#include "glew.h"
#include "MeshBuffers.h"
#include <map>
#include <string>

class MeshManager
{
public:
	MeshManager();
	~MeshManager();
	MeshBuffers* alreadyLoaded(char* name);
	void add(char*name, MeshBuffers* meshBuffers);
private:
	std::map<std::string, MeshBuffers*> meshes;
};

