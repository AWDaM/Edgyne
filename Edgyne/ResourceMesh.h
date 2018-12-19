#pragma once
#include "Resource.h"
#include "MathGeoLib/MathGeoLib.h"

class Vertex
{
public:
	Vertex()
	{}
	void Assign(vec position, float4 color)
	{
		this->position = position;
		this->color = color;
	}
	vec position = { 0,0,0 };
	float4 color = { 0,0,0,0 };
	float2 tex_coords = { 0,0 };
	vec normal = { 0,0,0 };
};
class ResourceMesh :
	public Resource
{
public:
	ResourceMesh(uint UID, ResourceType type);
	~ResourceMesh();

	void LoadResource(std::string& file);

public:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	uint id_vertex = 0;
	float* vertex = nullptr;

	uint true_vertex_index = 0;
	Vertex* true_vertex = nullptr;
	

	float* normals = nullptr;

	uint id_texture = 0;
	float* texCoords = nullptr;
	bool has_texture_coordinates = false;
	bool has_normals = false;
	bool has_triangle_faces = true;
	vec color;

	uint VBO;
	uint VAO;
	uint IBO;

	std::string material_name;
};

