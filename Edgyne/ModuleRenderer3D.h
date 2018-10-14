#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
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

struct mesh
{
	char name[25];

	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint id_normals = 0;
	float* normals = nullptr;

	uint id_texture = 0;
	float* texCoords = nullptr;

	AABB bounding_box;

	vec3 color = { 0,0,0 };

	vec3 position;
	vec3 rotation;
	vec3 scale;
	uint num_faces;


	bool Draw();
	void DrawNormals();
	void DrawBoundingBox();
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(rapidjson::Document& document);

	bool GenerateFramebuffer();

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);

	void OnResize(int width, int height);
	void Configuration();

	void CalculateGlobalBoundingBox();
	void DrawGlobalBoundingBox();

	void glSwitch(bool var, glRenderOptions option);
	void DeleteMesh();

public:

	uint framebuffer = 0;
	uint framebuffer_texture = 0;
	uint framebuffer_depth_and_stencil = 0;
	uint framebuffer_msaa = 0;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2d = true;
	bool line_smooth = false;
	bool scissor_test = false;

	bool draw_wireframe = false;

	uint  DroppedTexture;

	AABB globalBoundingBox;

	std::list<mesh*> mesh_list;
};