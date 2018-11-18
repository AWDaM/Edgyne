#include "ModuleResourceManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Resource.h"
#include "ResourceMaterial.h"


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "ResourceManager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}

Resource * ModuleResourceManager::GetResourceFromUID(uint uid)
{
	Resource* ret = nullptr;
	std::map<uint, Resource*>::iterator item = resources.find(uid);
	if (item != resources.end())
		return item->second;

	return ret;
}

Resource* ModuleResourceManager::CreateResource(ResourceType type, uint uid, rapidjson::Value::ConstMemberIterator comp)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case MATERIAL:
		 ret = new ResourceMaterial(uid,comp);
		break;
	case MESH:
		break;
	}
	resources.emplace(uid, ret);
	return ret;
}
