#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

class Material;
class Mesh;

struct MaterialResource
{
	std::string material_name;

	vec color = { 0,0,0 };
	float2 img_size = { 0,0 };
	uint id_texture = 0;
	std::string fileName;

	void GenerateMaterial(Material* buffer);
};

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

	void GenerateMesh(Mesh* buffer);
};
class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();
};

#endif
