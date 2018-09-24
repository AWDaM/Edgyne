#include "Application.h"
#include "ModuleImGui.h"
#include "GUIElements.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTest.h"
#include "pcg_variants.h"

#include <stdio.h>
#include <time.h>



ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "ImGui";
}



bool ModuleImGui::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	GUIElement.push_back(console = new GUIConsole());

	App->canLog = true;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();




	return true;
}

bool ModuleImGui::CleanUp()
{

	for (std::vector<GUIElements*>::iterator it = GUIElement.begin(); it != GUIElement.end(); ++it)
	{
		GUIElements* element = (*it);

		if (element->IsActive())
		{
			
		}
	}
	return false;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(App->window->window);
		ImGui::NewFrame();

	
	return status;
}

update_status ModuleImGui::Update(float dt)
{
	update_status status = UPDATE_CONTINUE;


	
	ImGui::BeginMainMenuBar();
	HelpMenu();
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("ExampleWindow", NULL, &show_demo_window);
			ImGui::MenuItem("RandomNumberTest", NULL, &show_random_number_test);
			ImGui::MenuItem("IntersectionsTest", NULL, &show_intersections_test);
			ImGui::MenuItem("Configuration", NULL, &show_configuration);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Console"))
		{
			ImGui::MenuItem("Show Console", NULL, &show_console);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Close"))
		{
			to_close = true;
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	
		for (std::vector<GUIElements*>::iterator it = GUIElement.begin(); it != GUIElement.end(); ++it)
		{
			GUIElements* element = (*it);

			if (element->IsActive())
			{
				ImGui::SetNextWindowPos(ImVec2((float)element->posx, (float)element->posy), ImGuiSetCond_Always);
				ImGui::SetNextWindowSize(ImVec2((float)element->width, (float)element->height), ImGuiSetCond_Always);
				element->Draw();
			}
		}

	if (show_demo_window)				ImGui::ShowDemoWindow(&show_demo_window);
	if (show_random_number_test)	RandomNumberTest();
	if (show_intersections_test)		IntersectionsTest();
	if (show_about_window)				About();
	if (show_console)							ConsoleWindow();
	if (show_configuration)				Configuration_window();

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

void ModuleImGui::RandomNumberTest()
{

	//Random Number without Limits
	ImGui::Begin("PCG Random Test", &perma_true_2);
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Normal Random Number");
	if (ImGui::Button("Generate Random Number"))
	{
		pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
	}
	ImGui::Text("%d", rng);


	//Random Number Between 0 And A Boundary
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Normal Random Number Between 0 and X");
	ImGui::InputInt("Min Number", &tmpBoundMin);
	ImGui::InputInt("Max Number", &tmpBoundMax);
	bound = tmpBoundMax - tmpBoundMin + 1;
	if (ImGui::Button("Generate Random Number With Boundaries"))
	{
		rng2 = pcg32_boundedrand_r(&dunnoWhatThisIs, bound);
		rng2 += tmpBoundMin;
	}
	ImGui::Text("%d", rng2);

	//Random Float Between 0 and 1
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Random Float Between 0 and 1");
	if (ImGui::Button("Generate Random Float Between 0 and 1"))
	{
		rng3 = ldexp(pcg32_random_r(&rngSeed3), -32);
	}
	ImGui::Text("%f", rng3);

	ImGui::End();
}

void ModuleImGui::IntersectionsTest()
{

	ImGui::Begin("Shapes", &perma_true);
	if (ImGui::TreeNode("Sphere"))
	{
		static float fx = 1.0f;
		static float fy = 1.0f;
		static float fz = 1.0f;
		static float fr = 1.0f;
		ImGui::InputFloat("X", &fx, 0.1f, 1.0f);
		ImGui::InputFloat("Y", &fy, 0.1f, 1.0f);
		ImGui::InputFloat("Z", &fz, 0.1f, 1.0f);
		ImGui::InputFloat("Radius", &fr, 0.1f, 1.0f);

		if (ImGui::Button("Spawn/Respawn Sphere"))
		{
			App->test->CreateSphere(fx, fy, fz, fr);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Capsule"))
	{
		static float fxt = 1.0f;
		static float fyt = 1.0f;
		static float fzt = 1.0f;
		static float fr = 1.0f;
		ImGui::InputFloat("top X", &fxt, 0.1f, 1.0f);
		ImGui::InputFloat("top Y", &fyt, 0.1f, 1.0f);
		ImGui::InputFloat("top Z", &fzt, 0.1f, 1.0f);


		static float fxb = 1.0f;
		static float fyb = 1.0f;
		static float fzb = 1.0f;
		ImGui::InputFloat("bottom X", &fxb, 0.1f, 1.0f);
		ImGui::InputFloat("bottom Y", &fyb, 0.1f, 1.0f);
		ImGui::InputFloat("bottom Z", &fzb, 0.1f, 1.0f);

		ImGui::InputFloat("Radius", &fr, 0.1f, 1.0f);

		if (ImGui::Button("Spawn/Respawn Capsule"))
		{
			App->test->CreateCapsule(fxt, fyt, fzt, fxb, fyb, fzb, fr);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("AABB"))
	{
		static float fxmax = 1.0f;
		static float fymax = 1.0f;
		static float fzmax = 1.0f;

		ImGui::InputFloat("maxX", &fxmax, 0.1f, 1.0f);
		ImGui::InputFloat("maxY", &fymax, 0.1f, 1.0f);
		ImGui::InputFloat("maxZ", &fzmax, 0.1f, 1.0f);

		static float fxmin = 1.0f;
		static float fymin = 1.0f;
		static float fzmin = 1.0f;

		ImGui::InputFloat("maxX", &fxmin, 0.1f, 1.0f);
		ImGui::InputFloat("maxY", &fymin, 0.1f, 1.0f);
		ImGui::InputFloat("maxZ", &fzmin, 0.1f, 1.0f);


		if (ImGui::Button("Spawn/Respawn AABB"))
		{
			App->test->CreateAABB(fxmax, fymax, fzmax, fxmin, fymin, fzmin);
		}
		ImGui::TreePop();

	}

	ImGui::End();

	ImGui::Begin("Intersections", &perma_true);
	ImGui::Checkbox("sphere", &App->test->sphere_intersection);
	LOG("%d", App->test->sphere_intersection);
	ImGui::Checkbox("capsule", &App->test->capsule_intersection);
	ImGui::Checkbox("aabb", &App->test->aabb_intersection);
	ImGui::End();

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

}

void ModuleImGui::ConsoleWindow()
{
	ImGui::Begin("Console", &show_console);
	{
		std::list<std::string>::iterator item = LogInformation.begin();
		while (item != LogInformation.end())
		{
			ImGui::Text(item->data());
			item++;
		}

	}
	ImGui::End();
}

void ModuleImGui::HelpMenu()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Documentation"))
			App->OpenBrowser("https://github.com/AWDaM/Edgyne/wiki");
		if (ImGui::MenuItem("Download last version"))
			App->OpenBrowser("https://github.com/AWDaM/Edgyne/releases");
		if (ImGui::MenuItem("Report a bug"))
			App->OpenBrowser("https://github.com/AWDaM/Edgyne/issues");
		if (ImGui::MenuItem("About"))
			show_about_window = true;
		ImGui::EndMenu();
	}
	


}

void ModuleImGui::About()
{

	ImGui::Begin("Edgyne", &show_about_window);
	ImGui::Dummy({ 0, 10 });
	ImGui::Text("Simple 3D engine developed for the Game Engines subject ");
	ImGui::Dummy({ 0, 10 });
	ImGui::Text("Authors: ");
	if (ImGui::Button("Lorien Portella Lopez"))					App->OpenBrowser("https://github.com/Witiza");
	if (ImGui::Button("Jaume Suris Valhondo "))				App->OpenBrowser("https://github.com/Farmak09");
	ImGui::Dummy({ 0, 10 });
	ImGui::Text("Libraries Used:");
	if (ImGui::Button("Simple Directmedia Layer 2.0.8"))	App->OpenBrowser("https://www.libsdl.org/");
	if (ImGui::Button("MathGeoLib 1.5"))							App->OpenBrowser("https://github.com/juj/MathGeoLib");
	ImGui::Dummy({ 0, 20 });
	ImGui::Text("License:");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("MIT License");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("Copyright(c) 2018 Jaume Surís Valhondo, Lorién Portella López");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
	ImGui::Text("of this software and associated documentation files(the Software), to deal");
	ImGui::Text("in the Software without restriction, including without limitation the rights");
	ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
	ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
	ImGui::Text("furnished to do so, subject to the following conditions :");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("The above copyright notice and this permission notice shall be included in all");
	ImGui::Text("copies or substantial portions of the Software.");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
	ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
	ImGui::Text("SOFTWARE.");


	ImGui::End();

}

void ModuleImGui::Configuration_window()
{
	if(ImGui::Begin("Configuration",&show_configuration))
	{ 
		if (ImGui::CollapsingHeader("General Options"))
		{
			ImGui::Button("Default Options");
			ImGui::SameLine();
			ImGui::Button("Load Options");
			ImGui::SameLine();
			ImGui::Button("Save Options");
		}
		std::list<Module*>::iterator item = App->list_modules.begin();

		while (item != App->list_modules.end())
		{
			(*item)->Configuration();
			item++;
		}
		
	}
	ImGui::End();
}

void ModuleImGui::AddLog(const char* Log)
{
	LogInformation.push_back(Log);

	console->AddLog(Log);
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


