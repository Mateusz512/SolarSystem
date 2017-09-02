#include "AlreadyLoadedHelper.h"



AlreadyLoadedHelper::AlreadyLoadedHelper()
{
}


AlreadyLoadedHelper::~AlreadyLoadedHelper()
{
}

MeshBuffers * AlreadyLoadedHelper::alreadyLoaded(char * name)
{
	auto found = meshes.find(std::string(name));
	if (found != meshes.end()) {
		return found->second;
	}
	return NULL;
}

void AlreadyLoadedHelper::add(char*name,MeshBuffers* meshBuffers)
{
	meshes[std::string(name)] = meshBuffers;
}
