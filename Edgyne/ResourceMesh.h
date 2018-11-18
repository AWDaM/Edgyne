#pragma once
#include "Resource.h"
class ResourceMesh :
	public Resource
{
public:
	ResourceMesh(std::string& file);
	ResourceMesh(rapidjson::Value::ConstMemberIterator comp);
	~ResourceMesh();

	void LoadResource(std::string& file);

public:
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
	vec color;

	std::string material_name;
};

