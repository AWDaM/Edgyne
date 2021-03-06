#include "Application.h"
#include "ModuleDebug.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "ModuleCamera3D.h"
#include "ModuleLevel.h"
#include "QuadTree.h"
#include "ImGuizmo\ImGuizmo.h"
#include "SDL\include\SDL_opengl.h"
#include <cmath>
#include <vector>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Debug";
}


ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Init(rapidjson::Value& node)
{
	return true;
}

bool ModuleDebug::Start()
{
	Vertex_Array_Cube();
	Indices_Array_Cube();

	const int CHECKERS_HEIGHT = 100;
	const int CHECKERS_WIDTH = 100;
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void ModuleDebug::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	
	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember("draw_wireframe", draw_wireframe, allocator);
	obj.AddMember("draw_plane", draw_plane, allocator);
	obj.AddMember("draw_axis", draw_axis, allocator);
	obj.AddMember("draw_normals", draw_normals, allocator);

	doc.AddMember((rapidjson::Value::StringRefType)name.data(), obj, allocator);

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
}

void ModuleDebug::Load(rapidjson::Document& doc)
{
	rapidjson::Value& node = doc[name.data()];

	draw_wireframe = node["draw_wireframe"].GetBool();
	if (draw_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_plane = node["draw_plane"].GetBool();
	draw_axis = node["draw_axis"].GetBool();
	draw_normals = node["draw_normals"].GetBool();

}

update_status ModuleDebug::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		operation_type = ImGuizmo::OPERATION::TRANSLATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_W))
	{
		operation_type = ImGuizmo::OPERATION::ROTATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E))
		operation_type = ImGuizmo::OPERATION::SCALE;

	return UPDATE_CONTINUE;
}

void ModuleDebug::Draw()
{

	glLineWidth(3.0f);

	if(App->level->selected_game_object)
		Draw_Guizmo(App->level->selected_game_object);

	if (draw_quadtree && !App->level->generate_quadtree)
		RecursiveDrawQuadtree(App->level->quad_tree->root_node);

	if (draw_plane)
		Draw_Plane();
	if (draw_axis)
		Draw_Axis();

	glColor3f(1.0f, 1.0, 1.0);
	
}

void ModuleDebug::Configuration()
{
	if (ImGui::CollapsingHeader("Debug"))
	{
		if (ImGui::Checkbox("WireframeRender", &draw_wireframe))
		{
			if (draw_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::Checkbox("Plane", &draw_plane);
		ImGui::Checkbox("Axis", &draw_axis);
		//ImGui::Checkbox("Normals", &draw_normals);
		ImGui::Checkbox("BoundingBoxes", &draw_boundingboxes);
		ImGui::Checkbox("Quad Tree", &draw_quadtree);
	}
}

void ModuleDebug::Draw_Plane()
{
	glBegin(GL_LINES);
	//-----Is this a plane?

	glColor3f(0.5f, 0.5f, 0.5f);
	float d = 50.0f;
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
}

void ModuleDebug::Draw_Axis()
{
	glLineWidth(3.0f);
	
	glBegin(GL_LINES);

	glColor3f(1.0f, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0.f, 1.0f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(0.f, 0.f, 1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glEnd();
	glLineWidth(1.0f);
}

void ModuleDebug::Draw_Normals(float * vertex, float * normals, int num_vertex)
{
	glLineWidth(2.0f);
	glColor3f(0, 1, 0);

	glBegin(GL_LINES);
	for (int i = 0; i < num_vertex * 3; i = i + 3)
	{
		glVertex3f(vertex[i] - normals[i], vertex[i + 1] - normals[i + 1], vertex[i + 2] - normals[i + 2]);
		glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
	}
	glEnd();

	glColor3f(1, 1, 1);

	glLineWidth(1.0f);
}

void ModuleDebug::RecursiveDrawQuadtree(const quadTreeNode * node) 
{
	Draw_AABB(node->boundingBox);

	if (node->isDivided)
	{
		for (int i = 0; i < 4; i++)
		{
			RecursiveDrawQuadtree(node->children[i]);
		}
	}
}

void ModuleDebug::Draw_Camera(const Camera * camera)
{
	glLineWidth(4.0f);
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);

	glVertex3f(camera->frustum.CornerPoint(0).x, camera->frustum.CornerPoint(0).y, camera->frustum.CornerPoint(0).z);
	glVertex3f(camera->frustum.CornerPoint(1).x, camera->frustum.CornerPoint(1).y, camera->frustum.CornerPoint(1).z);

	glVertex3f(camera->frustum.CornerPoint(0).x, camera->frustum.CornerPoint(0).y, camera->frustum.CornerPoint(0).z);
	glVertex3f(camera->frustum.CornerPoint(2).x, camera->frustum.CornerPoint(2).y, camera->frustum.CornerPoint(2).z);

	glVertex3f(camera->frustum.CornerPoint(0).x, camera->frustum.CornerPoint(0).y, camera->frustum.CornerPoint(0).z);
	glVertex3f(camera->frustum.CornerPoint(4).x, camera->frustum.CornerPoint(4).y, camera->frustum.CornerPoint(4).z);

	glVertex3f(camera->frustum.CornerPoint(7).x, camera->frustum.CornerPoint(7).y, camera->frustum.CornerPoint(7).z);
	glVertex3f(camera->frustum.CornerPoint(6).x, camera->frustum.CornerPoint(6).y, camera->frustum.CornerPoint(6).z);

	glVertex3f(camera->frustum.CornerPoint(6).x, camera->frustum.CornerPoint(6).y, camera->frustum.CornerPoint(6).z);
	glVertex3f(camera->frustum.CornerPoint(2).x, camera->frustum.CornerPoint(2).y, camera->frustum.CornerPoint(2).z);

	glVertex3f(camera->frustum.CornerPoint(7).x, camera->frustum.CornerPoint(7).y, camera->frustum.CornerPoint(7).z);
	glVertex3f(camera->frustum.CornerPoint(5).x, camera->frustum.CornerPoint(5).y, camera->frustum.CornerPoint(5).z);

	glVertex3f(camera->frustum.CornerPoint(7).x, camera->frustum.CornerPoint(7).y, camera->frustum.CornerPoint(7).z);
	glVertex3f(camera->frustum.CornerPoint(3).x, camera->frustum.CornerPoint(3).y, camera->frustum.CornerPoint(3).z);

	glVertex3f(camera->frustum.CornerPoint(3).x, camera->frustum.CornerPoint(3).y, camera->frustum.CornerPoint(3).z);
	glVertex3f(camera->frustum.CornerPoint(1).x, camera->frustum.CornerPoint(1).y, camera->frustum.CornerPoint(1).z);

	glVertex3f(camera->frustum.CornerPoint(1).x, camera->frustum.CornerPoint(1).y, camera->frustum.CornerPoint(1).z);
	glVertex3f(camera->frustum.CornerPoint(5).x, camera->frustum.CornerPoint(5).y, camera->frustum.CornerPoint(5).z);

	glVertex3f(camera->frustum.CornerPoint(3).x, camera->frustum.CornerPoint(3).y, camera->frustum.CornerPoint(3).z);
	glVertex3f(camera->frustum.CornerPoint(2).x, camera->frustum.CornerPoint(2).y, camera->frustum.CornerPoint(2).z);

	glVertex3f(camera->frustum.CornerPoint(4).x, camera->frustum.CornerPoint(4).y, camera->frustum.CornerPoint(4).z);
	glVertex3f(camera->frustum.CornerPoint(5).x, camera->frustum.CornerPoint(5).y, camera->frustum.CornerPoint(5).z);

	glVertex3f(camera->frustum.CornerPoint(6).x, camera->frustum.CornerPoint(6).y, camera->frustum.CornerPoint(6).z);
	glVertex3f(camera->frustum.CornerPoint(4).x, camera->frustum.CornerPoint(4).y, camera->frustum.CornerPoint(4).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}

void ModuleDebug::Draw_AABB(const AABB & box) 
{
	if (box.IsFinite())
	{
		glLineWidth(2.0f);
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);

		glVertex3f(box.CornerPoint(0).x, box.CornerPoint(0).y, box.CornerPoint(0).z);
		glVertex3f(box.CornerPoint(1).x, box.CornerPoint(1).y, box.CornerPoint(1).z);

		glVertex3f(box.CornerPoint(0).x, box.CornerPoint(0).y, box.CornerPoint(0).z);
		glVertex3f(box.CornerPoint(2).x, box.CornerPoint(2).y, box.CornerPoint(2).z);

		glVertex3f(box.CornerPoint(0).x, box.CornerPoint(0).y, box.CornerPoint(0).z);
		glVertex3f(box.CornerPoint(4).x, box.CornerPoint(4).y, box.CornerPoint(4).z);

		glVertex3f(box.CornerPoint(7).x, box.CornerPoint(7).y, box.CornerPoint(7).z);
		glVertex3f(box.CornerPoint(6).x, box.CornerPoint(6).y, box.CornerPoint(6).z);

		glVertex3f(box.CornerPoint(6).x, box.CornerPoint(6).y, box.CornerPoint(6).z);
		glVertex3f(box.CornerPoint(2).x, box.CornerPoint(2).y, box.CornerPoint(2).z);

		glVertex3f(box.CornerPoint(7).x, box.CornerPoint(7).y, box.CornerPoint(7).z);
		glVertex3f(box.CornerPoint(5).x, box.CornerPoint(5).y, box.CornerPoint(5).z);

		glVertex3f(box.CornerPoint(7).x, box.CornerPoint(7).y, box.CornerPoint(7).z);
		glVertex3f(box.CornerPoint(3).x, box.CornerPoint(3).y, box.CornerPoint(3).z);

		glVertex3f(box.CornerPoint(3).x, box.CornerPoint(3).y, box.CornerPoint(3).z);
		glVertex3f(box.CornerPoint(1).x, box.CornerPoint(1).y, box.CornerPoint(1).z);

		glVertex3f(box.CornerPoint(1).x, box.CornerPoint(1).y, box.CornerPoint(1).z);
		glVertex3f(box.CornerPoint(5).x, box.CornerPoint(5).y, box.CornerPoint(5).z);

		glVertex3f(box.CornerPoint(3).x, box.CornerPoint(3).y, box.CornerPoint(3).z);
		glVertex3f(box.CornerPoint(2).x, box.CornerPoint(2).y, box.CornerPoint(2).z);

		glVertex3f(box.CornerPoint(4).x, box.CornerPoint(4).y, box.CornerPoint(4).z);
		glVertex3f(box.CornerPoint(5).x, box.CornerPoint(5).y, box.CornerPoint(5).z);

		glVertex3f(box.CornerPoint(6).x, box.CornerPoint(6).y, box.CornerPoint(6).z);
		glVertex3f(box.CornerPoint(4).x, box.CornerPoint(4).y, box.CornerPoint(4).z);

		glEnd();
		glColor3f(1, 1, 1);
		glLineWidth(1.0f);
	}
}

void ModuleDebug::Draw_Guizmo(GameObject* game_object)
{

	/*float4x4 view_matrix = App->camera->editor_camera->frustum.ViewMatrix();
	float4x4 projection_matrix = App->camera->editor_camera->frustum.ProjectionMatrix();*/
	ImGuizmo::Enable(true);
	float4x4 view_matrix;
	float4x4 projection_matrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, (float*)view_matrix.v);
	glGetFloatv(GL_PROJECTION_MATRIX, (float*)projection_matrix.v);

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	
	float4x4 localMat = game_object->transform->LocalTransformationMatrix();
	localMat.Transpose();
	ImGuizmo::Manipulate((float*)view_matrix.v, (float*)projection_matrix.v, operation_type, ImGuizmo::WORLD, (float*)localMat.v,	NULL,NULL);
	localMat.Transpose();
	if (ImGuizmo::IsUsing())
	{
		usingGuizmo = true;
		if(operation_type == ImGuizmo::TRANSLATE)
			game_object->transform->position = localMat.TranslatePart();
		else if (operation_type == ImGuizmo::ROTATE)
		{
			game_object->transform->rotation = localMat.RotatePart().ToQuat();
			game_object->transform->rotation_euler = localMat.RotatePart().ToEulerXYZ();
			game_object->transform->rotation_euler.x *= RADTODEG;
			game_object->transform->rotation_euler.y *= RADTODEG;
			game_object->transform->rotation_euler.z *= RADTODEG;
		}
		else if(operation_type == ImGuizmo::SCALE)
			game_object->transform->scale = localMat.GetScale();

		game_object->RecursiveSetChildsTransformChanged(true);
	}
	else usingGuizmo = false;
	/*game_object->transform_changed = true;*/
}


void ModuleDebug::Vertex_Array_Cube()
{
	//1
	cube_vertex_array = {
	4, 0, 1,
	2, 0, 1,
	2, 0, -1,

	2, 0, -1,
	4, 0, -1,
	4, 0, 1,

	2, 0, 1,
	4, 0, 1,
	4, 2, 1,

	2, 0, 1,
	4, 2, 1,
	2, 2, 1,

	4, 2, 1,
	4, 0, 1,
	4, 2, -1,

	4, 2, -1,
	4, 0, 1,
	4, 0, -1,

	2, 2, 1,
	4, 2, -1,
	2, 2, -1,

	4, 2, 1,
	4, 2, -1,
	2, 2, 1,

	2, 2, 1,
	2, 0, -1,
	2, 0, 1,

	2, 2, -1,
	2, 0, -1,
	2, 2, 1,

	2, 2, -1,
	4, 2, -1,
	2, 0, -1,

	4, 0, -1,
	2, 0, -1,
	4, 2, -1 };

	glGenBuffers(1, (GLuint*)&(my_id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, my_id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &cube_vertex_array[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleDebug::Indices_Array_Cube()
{
	cube_indices_array = {
		-2, 0, 1,//B 0

		-4, 0, 1,//A 1

		-4, 0, -1, //E 2

		-2, 0, -1,//F 3

		-2, 2, 1, //D 4

		-4, 2, 1,//C 5

		-2, 2, -1,//H 6

		-4, 2, -1 }; //G 7

	cube_indices_indexes = {
		1,0,5,   5,0,4,   2,0,1,   2,3,0,   3,6,4,   3,4,0,   2,5,7,   5,2,1,   7,3,2,   7,6,3,   4,6,7,   7,5,4
	};
	glGenBuffers(1, (GLuint*)&(my_id_array));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id_array);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*36, &cube_indices_indexes[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
}
