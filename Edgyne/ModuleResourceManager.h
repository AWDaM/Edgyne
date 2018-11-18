#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include <map>
class Material;
class Mesh;
class Resource;

struct MeshResource
{
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	float* vertex = nullptr;

	float* normals = nullptr;

	uint id_texture = 0;
	float* texCoords = nullptr;
	bool has_texture_coordinates = false;
	bool has_normals = false;
	bool has_triangle_faces = true;

	std::string fileName;

	uint resource_uid = 0;
};
class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();
	
	Resource* GetResourceFromUID(uint uid);

	Resource* CreateResource(ResourceType type, uint uid, rapidjson::Value::ConstMemberIterator comp);

	std::map<uint, Resource*> resources;
};

#endif
