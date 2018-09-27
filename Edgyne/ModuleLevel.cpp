#include "Application.h"
#include "ModuleLevel.h"

#include "SDL\include\SDL_opengl.h"


ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(start_enabled)
{
}


ModuleLevel::~ModuleLevel()
{
}

void ModuleLevel::Draw()
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glVertex3f(0.f, 6.f, 3.f);
	glVertex3f(10.f, 0.f, 0.f);
	glVertex3f(0.f, 5.f, 10.f);
	glVertex3f(1.f, 0.f, 5.f);
	glVertex3f(0.f, 3.f, 0.f);
	glVertex3f(2.f, 3.f, 8.f);
	glVertex3f(10.f, 5.f, 0.f);
	glVertex3f(1.f, 10.f, 3.f);
	glVertex3f(0.f, 5.f, 7.f);
	glVertex3f(0.f, 3.f, 4.f);
	glVertex3f(8.f, 0.f, 6.f);
	glEnd();

	glLineWidth(1.0f);
}
