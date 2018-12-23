#include "ResourceMaterial.h"
#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleShaders.h"


ResourceMaterial::ResourceMaterial(std::string& file) : Resource(ResourceType::RES_MATERIAL,file)
{
	LoadMaterial(file);
}

ResourceMaterial::ResourceMaterial() : Resource(ResourceType::RES_MATERIAL)
{

}


ResourceMaterial::~ResourceMaterial()
{
}

void ResourceMaterial::LoadMaterial(std::string& path)
{
	file = path;

	shaderProgram = "Library\\ShaderPrograms\\DefaultProgram.edgyprogram";
	//color.Set(comp->value["Color"]["r"].GetFloat(), comp->value["Color"]["g"].GetFloat(), comp->value["Color"]["b"].GetFloat());

	App->loader->LoadTextureFromLibrary(file.c_str(), this);
}