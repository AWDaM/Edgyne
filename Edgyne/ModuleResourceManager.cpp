#include "ModuleResourceManager.h"
#include "Application.h"
#include "Material.h"
#include "Mesh.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "ResourceManager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}

Resource * ModuleResourceManager::GetResourceFromUID(const uint& uid) 
{
	Resource* ret = nullptr;
	std::map<uint, Resource*>::iterator item = resources.find(uid);
	if (item != resources.end())
		return item->second;

	return ret;
}

Resource* ModuleResourceManager::CreateResource(Resource::ResourceType type)
{
	Resource* ret = nullptr;
	uint uid = pcg32_random_r(&App->rng);

	switch (type)
	{
	case Resource::ResourceType::RES_MATERIAL:
		 ret = new ResourceMaterial();
		break;
	case Resource::ResourceType::RESOURCE_MESH:
		ret = new ResourceMesh();
		break;
	}
	resources.emplace(uid, ret);
	return ret;
}

Resource* ModuleResourceManager::CreateNewResource(uint& uid, Resource::ResourceType type, std::string & file)
{
	Resource* ret = nullptr;
	uint uid = pcg32_random_r(&App->rng);

	switch (type)
	{
	case Resource::RES_MATERIAL:
		ret = new ResourceMaterial(uid, file);
		break;
	case Resource::RESOURCE_MESH:
		ret = new ResourceMesh(uid, file);
		break;
	}
	resources.emplace(uid, ret);
		return ret;
}


//Resource* ModuleResourceManager::CreateNewResource(Resource::ResourceType type, const std::string& file)
//{
//	Resource* ret = nullptr;
//	switch (type)
//	{
//	case Resource::RES_MATERIAL:
//		ret = new ResourceMaterial(file);
//		break;
//	case Resource::RESOURCE_MESH:
//		ret = new ResourceMesh(file);
//		break;
//		resources.emplace(ret->file, ret);
//		return ret;
//	}
//}


