#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "ModuleLevel.h"
#include "ModuleDebug.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Render3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(rapidjson::Document& document)
{
	App->Log("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		LOG("Glew library not initialized properly %s\n", glewGetErrorString(err));
		ret = false;
	}
	
	if(ret == true)
	{
		
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float*)App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

	update_status status = UPDATE_CONTINUE;

	App->level->Draw();

	App->debug->Draw();


	std::list<mesh*>::iterator item = mesh_list.begin();

	while (item != mesh_list.end())
	{
		(*item)->Draw();

		item++;
	}


	App->imGui->Draw();

	if (App->imGui->to_close == true) // A bit hardcoded, but cant find any other way
	{
		status = UPDATE_STOP;
	}

	SDL_GL_SwapWindow(App->window->window);
	return status;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::Configuration()
{
	if (ImGui::CollapsingHeader("Render3D"))
	{
		if (ImGui::Checkbox("Depth Test", &depth_test))
			glSwitch(depth_test, DEPTH_TEST);
		if (ImGui::Checkbox("Cull Face", &cull_face))
			glSwitch(cull_face, CULL_FACE);
		if (ImGui::Checkbox("Lighting", &lighting))
			glSwitch(lighting, LIGHTING);
		if (ImGui::Checkbox("Color Material", &color_material))
			glSwitch(color_material, COLOR_MATERIAL);
		if (ImGui::Checkbox("Texture 2D", &texture_2d))
			glSwitch(texture_2d, TEXTURE_2D);
		if (ImGui::Checkbox("Line Smooth", &line_smooth))
			glSwitch(line_smooth, LINE_SMOOTH);
		if (ImGui::Checkbox("Scissor Test", &scissor_test))
			glSwitch(scissor_test, SCISSOR_TEST);
	}
}

void ModuleRenderer3D::glSwitch(bool var, glRenderOptions option)
{
	switch (option)
	{
		case 0:
		{
			LOG("Invalid RenderOption");
		}
			break;
		case 1:
		{
			if (var)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}
			break;
		case 2:
		{
			if (var)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);
		}
			break;
		case 3:
		{
			if (var)
				glEnable(GL_LIGHTING);
			else
				glDisable(GL_LIGHTING);
		}
			break;
		case 4: 
		{
			if (var)
				glEnable(GL_COLOR_MATERIAL);
			else
				glDisable(GL_COLOR_MATERIAL);
		}
			break;
		case 5:
		{
			if (var)
				glEnable(GL_TEXTURE_2D);
			else
				glDisable(GL_TEXTURE_2D);
		}
			break;
		case 6:
		{
			if (var)
				glEnable(GL_LINE_SMOOTH);
			else
				glDisable(GL_LINE_SMOOTH);
		}
			break;
		case 7:
		{
			if (var)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
		}
			break;

	}

}

bool mesh::Draw()
{
	//Enable All The Data
	glEnableClientState(GL_VERTEX_ARRAY);

	 if (id_texture)
	{
		glBindTexture(GL_TEXTURE_2D, id_texture);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);

	//Draw The Mesh
	glDrawElements(GL_TRIANGLES,num_index, GL_UNSIGNED_INT, NULL);

	DrawNormals();

	//Disable All The Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (id_texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


	return true;
}

void mesh::DrawNormals()
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
