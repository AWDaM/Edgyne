#include "Application.h"
#include "ModuleDebug.h"

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "debug";
}


ModuleDebug::~ModuleDebug()
{
}


bool ModuleDebug::Start()
{
	Vertex_Array_Cube();
	Indices_Array_Cube();
	return true;
}

update_status ModuleDebug::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleDebug::Draw()
{

	glLineWidth(3.0f);


	if (direct_mode_cube)
	{
		Draw_Cube_Direct_Mode();
	}
	if (draw_plane)
		Draw_Plane();
	if (draw_axis)
		Draw_Axis();

	Draw_Cube_Vertex();
	glColor3f(1.0f, 0, 0);
	Draw_Cube_Indices();
		
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
		ImGui::Checkbox("Direct Mode Cube", &direct_mode_cube);
		ImGui::Checkbox("Plane", &draw_plane);
		ImGui::Checkbox("Axis", &draw_axis);
	}
}

void ModuleDebug::Draw_Cube_Direct_Mode()
{
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(1, 0, 1);
	glVertex3f(-1, 0, 1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 0, -1);
	glVertex3f(1, 0, -1);
	glVertex3f(1, 0, 1);

	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 2, 1);

	glVertex3f(-1, 0, 1);
	glVertex3f(1, 2, 1);
	glVertex3f(-1, 2, 1);

	glVertex3f(1, 2, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 2, -1);

	glVertex3f(1, 2, -1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, -1);

	glVertex3f(-1, 2, 1);
	glVertex3f(1, 2, -1);
	glVertex3f(-1, 2, -1);

	glVertex3f(1, 2, 1);
	glVertex3f(1, 2, -1);
	glVertex3f(-1, 2, 1);

	glVertex3f(-1, 2, 1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 0, 1);

	glVertex3f(-1, 2, -1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 2, 1);

	glVertex3f(-1, 2, -1);
	glVertex3f(1, 2, -1);
	glVertex3f(-1, 0, -1);

	glVertex3f(1, 0, -1);
	glVertex3f(-1, 0, -1);
	glVertex3f(1, 2, -1);

	glEnd();
}

void ModuleDebug::Draw_Plane()
{
	glBegin(GL_LINES);
	//-----Is this a plane?

	glColor3f(0.5f, 0.5f, 0.5f);
	float d = 20.0f;
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

void ModuleDebug::Draw_Cube_Vertex()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleDebug::Draw_Cube_Indices()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id_array);

	glVertexPointer(3, GL_FLOAT, 0, &cube_indices_array[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
