#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"



ResourceMesh::ResourceMesh(std::string& file) : Resource(ResourceType::RESOURCE_MESH,file)
{
	LoadResource(file);
}

ResourceMesh::ResourceMesh(rapidjson::Value::ConstMemberIterator comp) : Resource(ResourceType::RESOURCE_MESH)
{
	//LoadComponent(comp);
}



ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::LoadResource(std::string& path)
{
	file = path;
	std::string tmp = "Library\\Meshes\\";

	tmp += file;
	
	App->importer->CopyDataFromFile(tmp, this);
}
