#include "ResourceMaterial.h"
#include "Application.h"
#include "ModuleLoader.h"


ResourceMaterial::ResourceMaterial(uint uid, rapidjson::Value::ConstMemberIterator comp) : Resource(uid, ResourceType::MATERIAL)
{
	LoadComponent(comp);
}


ResourceMaterial::~ResourceMaterial()
{
}

void ResourceMaterial::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	fileName = comp->value["Texture Name"].GetString();

	color.Set(comp->value["Color"]["r"].GetFloat(), comp->value["Color"]["g"].GetFloat(), comp->value["Color"]["b"].GetFloat());

	App->loader->LoadTextureFromLibrary(fileName.c_str(), this);
}