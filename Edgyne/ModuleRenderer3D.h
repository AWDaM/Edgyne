#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#define MAX_LIGHTS 8

enum glRenderOptions
{
	DEFAULT = 0,
	DEPTH_TEST,
	CULL_FACE,
	LIGHTING,
	COLOR_MATERIAL,
	TEXTURE_2D,
	LINE_SMOOTH,
	SCISSOR_TEST
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void Configuration();

	void glSwitch(bool var, glRenderOptions option);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	math::float3x3 NormalMatrix;
	math::float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = false;
	bool line_smooth = false;
	bool scissor_test = false;

	bool draw_wireframe = false;
};