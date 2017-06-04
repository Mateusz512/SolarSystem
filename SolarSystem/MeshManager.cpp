#include "MeshManager.h"



MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
}

MeshBuffers * MeshManager::alreadyLoaded(char * name)
{
	auto found = meshes.find(std::string(name));
	if (found != meshes.end()) {
		return found->second;
	}
	return NULL;
}

void MeshManager::add(char*name,MeshBuffers* meshBuffers)
{
	meshes[std::string(name)] = meshBuffers;
}
