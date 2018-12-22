#ifndef __MODULE_RENDERER_3D_H__
#define __MODULE_RENDERER_3D_H__


#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "SDL/include/SDL.h"

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
	SCISSOR_TEST,
	BLEND
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(rapidjson::Value& node);

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);

	bool GenerateFramebuffer();

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


	void OnResize(int width, int height);
	void Configuration();

	void DrawMainCameraFrustum();

	void glSwitch(bool var, glRenderOptions option);
	
public:

	uint framebuffer = 0;
	uint framebuffer_texture = 0;
	uint framebuffer_depth_and_stencil = 0;
	uint framebuffer_msaa = 0;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool line_smooth = false;
	bool scissor_test = false;
	bool camera_culling = false;
	bool blend = true;

	uint  DroppedTexture;

	AABB globalBoundingBox;
};

#endif // !__MODULE_RENDERER_3D_H__