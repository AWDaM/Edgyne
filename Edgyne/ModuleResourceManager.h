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
	
	uint ImportFile(const char* file, Resource::ResourceType type);
	JSON_File* getMeta(const char* path) const;
	JSON_File* createMeta(const char* path, Resource::ResourceType type) const;
	int GetMetaLastChange(const char* path) const;

	bool wasFileChanged(const char* file) const;

	Resource* GetResourceFromUID(uint& UID);
	Resource* AddResource(uint& UID, Resource::ResourceType type);
	bool DeleteResource(uint uid);
	bool UpdateMetaLastChange(const char* path);
	//Resource* CreateNewResource(uint& UID, Resource::ResourceType type, std::string& file);

	std::map<uint, Resource*> resources;
};

#endif
