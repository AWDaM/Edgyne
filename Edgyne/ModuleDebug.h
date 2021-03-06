#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__


#include "Module.h"
#include <array>
#include "GL/glew.h"
#include "ImGuizmo/ImGuizmo.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Camera;
class quadTreeNode;
class GameObject;


class ModuleDebug :
	public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();
	bool Init(rapidjson::Value& node);

	bool Start();
	update_status Update(float dt);
	void Draw();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);

	void Configuration();
	void Draw_Cube_Direct_Mode();
	void Draw_Plane();
	void Draw_Axis();
	void Draw_Sphere();
	void Draw_Cube_Vertex();
	void Draw_Cube_Indices();
	void Draw_Normals(float* vertex,float*normals,int num_vertex);

	void RecursiveDrawQuadtree(const quadTreeNode* node);

	void Draw_Camera(const Camera* camera);
	void Draw_AABB(const AABB& box);

	void Draw_Guizmo(GameObject* game_object);

	void Vertex_Array_Cube();
	void Indices_Array_Cube();

public:
	bool draw_wireframe = false;
	bool draw_plane = true;
	bool draw_axis = true;
	bool draw_normals = false;
	bool draw_quadtree = false;
	bool draw_boundingboxes = false;
	
	ImGuizmo::OPERATION operation_type;
	

	uint my_id_vertex = 0;
	uint my_id_array = 0;
	

	uint my_id = 0;
	float4x4 transform;
	bool axis, wire;

	std::vector<float> shape;
	std::vector<uint> indicesS;

	float radius = 1;
	int rings = 6;
	int sectors = 12;
	vec pos = { 0,3,0 };
	std::array<float, 108> cube_vertex_array;
	std::array<float, 24> cube_indices_array;
	std::array<uint, 36> cube_indices_indexes;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;

	GLubyte checkImage[100][100][4];
	uint ImageName = 0;
	bool toDrawSphere = true;
	bool usingGuizmo = false;
};

#endif // !__MODULE_DEBUG_H__