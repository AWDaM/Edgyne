#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "ModuleLevel.h"
#include "ModuleDebug.h"
#include "Camera.h"
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
bool ModuleRenderer3D::Init(rapidjson::Value& node)
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
		App->vsync_on = node["VSync"].GetBool();
		if(App->vsync_on && SDL_GL_SetSwapInterval(1) < 0)
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
	OnResize(App->window->window_w, App->window->window_h);
	GenerateFramebuffer();

	return ret;
}

bool ModuleRenderer3D::GenerateFramebuffer()
{
	bool ret = true;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	glGenTextures(1, &framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,App->window->window_w, App->window->window_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);


	glGenRenderbuffers(1, &framebuffer_depth_and_stencil);
	glBindRenderbuffer(GL_RENDERBUFFER, framebuffer_depth_and_stencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->window_w, App->window->window_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer_depth_and_stencil);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Problem generating framebuffer: %s", glGetError());
		ret = false;
	}


	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!App->imGui->EditorOff)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, App->window->window_w, App->window->window_h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->editor_camera->GetViewMatrix());

	// light 0 on cam pos
	//lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	//for(uint i = 0; i < MAX_LIGHTS; ++i)
	//	lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

	update_status status = UPDATE_CONTINUE;

	App->level->Draw();

	App->debug->Draw();



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
	glDeleteFramebuffers(1, &framebuffer);
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	App->window->window_w = width;
	App->window->window_h = height;
	GenerateFramebuffer();

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	App->camera->editor_camera->aspect_ratio = (float)App->window->window_w / App->window->window_h;
	App->camera->editor_camera->ChangeFrustum();
	glLoadMatrixf(App->camera->editor_camera->GetProjectionMatrix());
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void ModuleRenderer3D::Configuration()
{
	if (ImGui::CollapsingHeader("Render3D"))
	{
		if(ImGui::Checkbox("CameraCulling",&camera_culling))
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


void ModuleRenderer3D::DrawMainCameraFrustum()
{
	glLineWidth(4.0f);
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);

	glVertex3f(globalBoundingBox.CornerPoint(0).x, globalBoundingBox.CornerPoint(0).y, globalBoundingBox.CornerPoint(0).z);
	glVertex3f(globalBoundingBox.CornerPoint(1).x, globalBoundingBox.CornerPoint(1).y, globalBoundingBox.CornerPoint(1).z);

	glVertex3f(globalBoundingBox.CornerPoint(0).x, globalBoundingBox.CornerPoint(0).y, globalBoundingBox.CornerPoint(0).z);
	glVertex3f(globalBoundingBox.CornerPoint(2).x, globalBoundingBox.CornerPoint(2).y, globalBoundingBox.CornerPoint(2).z);

	glVertex3f(globalBoundingBox.CornerPoint(0).x, globalBoundingBox.CornerPoint(0).y, globalBoundingBox.CornerPoint(0).z);
	glVertex3f(globalBoundingBox.CornerPoint(4).x, globalBoundingBox.CornerPoint(4).y, globalBoundingBox.CornerPoint(4).z);

	glVertex3f(globalBoundingBox.CornerPoint(7).x, globalBoundingBox.CornerPoint(7).y, globalBoundingBox.CornerPoint(7).z);
	glVertex3f(globalBoundingBox.CornerPoint(6).x, globalBoundingBox.CornerPoint(6).y, globalBoundingBox.CornerPoint(6).z);

	glVertex3f(globalBoundingBox.CornerPoint(6).x, globalBoundingBox.CornerPoint(6).y, globalBoundingBox.CornerPoint(6).z);
	glVertex3f(globalBoundingBox.CornerPoint(2).x, globalBoundingBox.CornerPoint(2).y, globalBoundingBox.CornerPoint(2).z);

	glVertex3f(globalBoundingBox.CornerPoint(7).x, globalBoundingBox.CornerPoint(7).y, globalBoundingBox.CornerPoint(7).z);
	glVertex3f(globalBoundingBox.CornerPoint(5).x, globalBoundingBox.CornerPoint(5).y, globalBoundingBox.CornerPoint(5).z);

	glVertex3f(globalBoundingBox.CornerPoint(7).x, globalBoundingBox.CornerPoint(7).y, globalBoundingBox.CornerPoint(7).z);
	glVertex3f(globalBoundingBox.CornerPoint(3).x, globalBoundingBox.CornerPoint(3).y, globalBoundingBox.CornerPoint(3).z);

	glVertex3f(globalBoundingBox.CornerPoint(3).x, globalBoundingBox.CornerPoint(3).y, globalBoundingBox.CornerPoint(3).z);
	glVertex3f(globalBoundingBox.CornerPoint(1).x, globalBoundingBox.CornerPoint(1).y, globalBoundingBox.CornerPoint(1).z);

	glVertex3f(globalBoundingBox.CornerPoint(1).x, globalBoundingBox.CornerPoint(1).y, globalBoundingBox.CornerPoint(1).z);
	glVertex3f(globalBoundingBox.CornerPoint(5).x, globalBoundingBox.CornerPoint(5).y, globalBoundingBox.CornerPoint(5).z);

	glVertex3f(globalBoundingBox.CornerPoint(3).x, globalBoundingBox.CornerPoint(3).y, globalBoundingBox.CornerPoint(3).z);
	glVertex3f(globalBoundingBox.CornerPoint(2).x, globalBoundingBox.CornerPoint(2).y, globalBoundingBox.CornerPoint(2).z);

	glVertex3f(globalBoundingBox.CornerPoint(4).x, globalBoundingBox.CornerPoint(4).y, globalBoundingBox.CornerPoint(4).z);
	glVertex3f(globalBoundingBox.CornerPoint(5).x, globalBoundingBox.CornerPoint(5).y, globalBoundingBox.CornerPoint(5).z);

	glVertex3f(globalBoundingBox.CornerPoint(6).x, globalBoundingBox.CornerPoint(6).y, globalBoundingBox.CornerPoint(6).z);
	glVertex3f(globalBoundingBox.CornerPoint(4).x, globalBoundingBox.CornerPoint(4).y, globalBoundingBox.CornerPoint(4).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
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

void ModuleRenderer3D::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember("depth_test", depth_test, allocator);
	obj.AddMember("cull_face", cull_face, allocator);
	obj.AddMember("lighting", lighting, allocator);
	obj.AddMember("color_material", color_material, allocator);
	obj.AddMember("texture_2d", texture_2d, allocator);
	obj.AddMember("line_smooth", line_smooth, allocator);
	obj.AddMember("scissor_test", scissor_test, allocator);

	doc.AddMember((rapidjson::Value::StringRefType)name.data(), obj, allocator);

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
}

void ModuleRenderer3D::Load(rapidjson::Document& doc)
{
	rapidjson::Value& node = doc[name.data()];

	depth_test = node["depth_test"].GetBool();
	glSwitch(depth_test, DEPTH_TEST);

	cull_face = node["cull_face"].GetBool();
	glSwitch(cull_face, CULL_FACE);

	lighting = node["lighting"].GetBool();
	glSwitch(lighting, LIGHTING);

	color_material = node["color_material"].GetBool();
	glSwitch(color_material, COLOR_MATERIAL);

	texture_2d = node["texture_2d"].GetBool();
	glSwitch(texture_2d, TEXTURE_2D);

	line_smooth = node["line_smooth"].GetBool();
	glSwitch(line_smooth, LINE_SMOOTH);

	scissor_test = node["scissor_test"].GetBool();
	glSwitch(scissor_test, SCISSOR_TEST);
}