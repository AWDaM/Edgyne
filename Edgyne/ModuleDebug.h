#pragma once
#include "Module.h"
#include <array>


class ModuleDebug :
	public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();

	bool Start();
	update_status Update(float dt);
	void Draw();
	void Configuration();
	void Draw_Cube_Direct_Mode();
	void Draw_Plane();
	void Draw_Axis();
	void Draw_Cube_Vertex_Array();

	void Vertex_Array_Cube();
	void Indices_Array_Cube();

public:
	bool draw_wireframe = false;
	bool direct_mode_cube = true;
	bool draw_plane = true;
	bool draw_axis = true;

	uint my_id_vertex = 0;

	std::array<float, 108> cube_vertex_array;
};

