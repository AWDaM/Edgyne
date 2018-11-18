#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include "MathGeoLib\MathGeoLib.h"
#include <map>
class Material;
class Mesh;
class Resource;



class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();
	
	Resource* GetResourceFromUID(const std::string & name);

	Resource* CreateResource(Resource::ResourceType type, rapidjson::Value::ConstMemberIterator comp);

	Resource* CreateNewResource(Resource::ResourceType type, std::string& file);

	std::map<std::string, Resource*> resources;
};

#endif
