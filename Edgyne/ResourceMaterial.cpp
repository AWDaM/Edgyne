#include "ResourceMaterial.h"
#include "Application.h"
#include "ModuleLoader.h"


ResourceMaterial::ResourceMaterial(uint UID, std::string& file) : Resource(UID, ResourceType::RES_MATERIAL,file)
{
	LoadMaterial(file);
}



ResourceMaterial::~ResourceMaterial()
{
}

void ResourceMaterial::LoadMaterial(std::string& path)
{
	file = path;

	//color.Set(comp->value["Color"]["r"].GetFloat(), comp->value["Color"]["g"].GetFloat(), comp->value["Color"]["b"].GetFloat());

	App->loader->LoadTextureFromLibrary(file.c_str(), this);
}