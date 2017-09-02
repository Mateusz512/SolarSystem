#pragma once
#include "glew.h"
#include "MeshBuffers.h"
#include <map>
#include <string>

class AlreadyLoadedHelper
{
public:
	AlreadyLoadedHelper();
	~AlreadyLoadedHelper();
	MeshBuffers* alreadyLoaded(char* name);
	void add(char*name, MeshBuffers* meshBuffers);
private:
	std::map<std::string, MeshBuffers*> meshes;
};

