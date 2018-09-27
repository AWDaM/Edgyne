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
	/*glLineWidth(2.0f);

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

	glLineWidth(1.0f);*/

	glLineWidth(3.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
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
