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
	
	Resource* GetResourceFromUID(const uint& uid);

	Resource* CreateResource(Resource::ResourceType type);

	Resource* CreateNewResource(uint& uid, Resource::ResourceType type, std::string& file);

	std::map<uint, Resource*> resources;
};

#endif
