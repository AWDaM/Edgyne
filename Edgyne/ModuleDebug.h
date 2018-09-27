#pragma once
#include "Module.h"
class ModuleDebug :
	public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();

	update_status Update(float dt);
	void Draw();
	void Configuration();
	void Draw_Cube_Direct_Mode();
	void Draw_Plane();
	void Draw_Axis();

public:
	bool draw_wireframe = false;
	bool direct_mode_cube = false;
	bool draw_plane = true;
	bool draw_axis = true;
};

