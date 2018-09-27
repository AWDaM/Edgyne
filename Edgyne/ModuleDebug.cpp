#include "Application.h"
#include "ModuleDebug.h"

#include "SDL\include\SDL_opengl.h"


ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "debug";
}


ModuleDebug::~ModuleDebug()
{
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
	glLineWidth(1.0f);
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
}
