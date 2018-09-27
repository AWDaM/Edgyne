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
	//glLineWidth(2.0f);

	//glBegin(GL_LINES);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(10.f, 0.f, 0.f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 0.f, 10.f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 10.f, 0.f);
	//glVertex3f(10.f, 10.f, 0.f);
	//glVertex3f(0.f, 10.f, 10.f);
	//glVertex3f(10.f, 10.f, 10.f);
	//glVertex3f(0.f, 0.f, 10.f);
	//glVertex3f(10.f, 0.f, 10.f);
	//glVertex3f(10.f, 0.f, 0.f);
	//glVertex3f(10.f, 0.f, 0.f);
	//glVertex3f(10.f, 0.f, 0.f);
	//glVertex3f(10.f, 0.f, 0.f);
	//glVertex3f(10.f, 0.f, 0.f);
	//glEnd();

	//glLineWidth(1.0f);
}
