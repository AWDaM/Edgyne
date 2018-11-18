#include "ResourceMaterial.h"
#include "Application.h"
#include "ModuleLoader.h"


ResourceMaterial::ResourceMaterial(std::string& file) : Resource(ResourceType::RES_MATERIAL,file)
{
	LoadMaterial(file);
}

ResourceMaterial::ResourceMaterial(rapidjson::Value::ConstMemberIterator comp) : Resource(ResourceType::RES_MATERIAL)
{
	//LoadComponent(comp);
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