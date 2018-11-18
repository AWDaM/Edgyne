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

Resource * ModuleResourceManager::GetResourceFromUID(const std::string& uid) 
{
	Resource* ret = nullptr;
	std::map<std::string, Resource*>::iterator item = resources.find(uid);
	if (item != resources.end())
		return item->second;

	return ret;
}

Resource* ModuleResourceManager::CreateResource(Resource::ResourceType type, rapidjson::Value::ConstMemberIterator comp)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case Resource::ResourceType::RES_MATERIAL:
		 ret = new ResourceMaterial(comp);
		break;
	case Resource::ResourceType::RESOURCE_MESH:
		ret = new ResourceMesh(comp);
		break;
	}
	resources.emplace(ret->file, ret);
	return ret;
}

Resource * ModuleResourceManager::CreateNewResource(Resource::ResourceType type,std::string & file)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case Resource::RES_MATERIAL:
		ret = new ResourceMaterial(file);
		break;
	case Resource::RESOURCE_MESH:
		ret = new ResourceMesh(file);
		break;
	}
	resources.emplace(ret->file, ret);
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


