#pragma once
#include "Component.h"
#include "Globals.h"

class Mesh : public Component
{
public:
	Mesh();
	Mesh(GameObject* game_object);
	~Mesh();

	void OnEditor();

	void SetBoundingVolume();

private:
public:

private:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	float* vertex = nullptr;

	uint id_normals = 0;
	float* normals = nullptr;

	uint id_texture = 0;
	float* texCoords = nullptr;
};

