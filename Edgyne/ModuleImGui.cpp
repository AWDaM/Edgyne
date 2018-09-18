#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include <stdio.h>





ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}



bool ModuleImGui::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();




	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(App->window->window);
		ImGui::NewFrame();

		math::vec test = math::vec(5, 5, 5);
		LOG("%f",test.x);
	
	return status;
}

update_status ModuleImGui::Update(float dt)
{
	update_status status = UPDATE_CONTINUE;

	

	
	ImGui::BeginMainMenuBar();
	
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("ExampleWindow", NULL, &show_demo_window);
		
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Close"))
		{
			to_close = true;
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}
	
	return status;
}

update_status ModuleImGui::PostUpdate(float dt)
{

	
	ImGui::Render();
	
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	update_status status = UPDATE_CONTINUE;
	if (to_close == true)
	{
		status = UPDATE_STOP;
	}
	return status;
}


ModuleImGui::~ModuleImGui()
{
	ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	
		SDL_GL_DeleteContext(App->renderer3D->context);
		SDL_DestroyWindow(App->window->window);
		SDL_Quit();
	
}


