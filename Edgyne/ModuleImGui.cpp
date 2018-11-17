#include "Application.h"
#include "ModuleImGui.h"
#include "GUIElements.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ImGuizmo\ImGuizmo.h"

#include "GUIConsole.h"
#include "GUIAbout.h"
#include "GUIConfiguration.h"
#include "GUIRandomNumberTest.h"
#include "GUIInspector.h"
#include "GUIScene.h"
#include "GUIHierarchy.h"
#include "GUIAssets.h"
#include <stdio.h>




ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "ImGui";
}



bool ModuleImGui::Init(rapidjson::Value& node)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;  // Enable Keyboard Controls
	ImGuiIniLocation = ("Settings\\");
	ImGuiIniLocation.append(io.IniFilename);
	io.IniFilename = ImGuiIniLocation.c_str();
	ImGui::LoadIniSettingsFromDisk(io.IniFilename);
	//ImGuiIniLocation.("Settings\\");
	//io.IniFilename = ImGuiIniLocation.c_str();
	

	GUIElement.push_back(console = new GUIConsole(App->log, true));
	GUIElement.push_back(about = new GUIAbout(false));
	GUIElement.push_back(configuration = new GUIConfiguration(true));
	GUIElement.push_back(random_number_test = new GUIRandomNumberTest(false));
	GUIElement.push_back(inspector = new GUIInspector(true));
	GUIElement.push_back(scene = new GUIScene(true));
	GUIElement.push_back(hierarchy = new GUIHierarchy(true));
	GUIElement.push_back(assets = new GUIAssets(true));

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

	}
	return false;
}

void ModuleImGui::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value Obj(rapidjson::kObjectType);

	for (std::vector<GUIElements*>::iterator it = GUIElement.begin(); it != GUIElement.end(); ++it)
	{
		rapidjson::Value childrenData(rapidjson::kObjectType);

		(*it)->Save(childrenData, allocator);

		Obj.AddMember((rapidjson::Value::StringRefType)(*it)->name.data(), childrenData, allocator);

	}
	doc.AddMember((rapidjson::Value::StringRefType)name.data(), Obj, allocator);

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

}

void ModuleImGui::Load(rapidjson::Document& doc)
{
	rapidjson::Value& node = doc[name.data()];
	for (std::vector<GUIElements*>::iterator it = GUIElement.begin(); it != GUIElement.end(); ++it)
	{
		(*it)->Load(node[(*it)->name.data()]);
	}

}

update_status ModuleImGui::PreUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;


		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(App->window->window);
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruDockspace;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Edgyne", &perma_true, window_flags);
		ImGui::PopStyleVar(3);
		ImGuiIO& io = ImGui::GetIO();

		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

	return status;
}

update_status ModuleImGui::Update(float dt)
{
	update_status status = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		ToggleEditor();

	return status;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;


	return status;
}

void ModuleImGui::Draw()
{
	if (!EditorOff)
	{
		MainMenu();
		for (std::vector<GUIElements*>::iterator it = GUIElement.begin(); it != GUIElement.end(); ++it)
		{
			GUIElements* element = (*it);

			if (element->IsActive())
			{
				element->Draw();
			}
		}
		ImGui::End();
	}
		ImGui::Render();

		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
}





void ModuleImGui::HelpMenu()
{

		if (ImGui::MenuItem("Documentation"))
			App->OpenBrowser("https://github.com/AWDaM/Edgyne/wiki");
		if (ImGui::MenuItem("Download last version"))
			App->OpenBrowser("https://github.com/AWDaM/Edgyne/releases");
		if (ImGui::MenuItem("Report a bug"))
			App->OpenBrowser("https://github.com/AWDaM/Edgyne/issues");
		if (ImGui::MenuItem("About"))
			about->ToggleActive();

}


void ModuleImGui::MainMenu()
{ 

		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Scene", NULL, &scene->active);
			ImGui::MenuItem("ExampleWindow", NULL, &show_demo_window);
			ImGui::MenuItem("RandomNumberTest", NULL, &random_number_test->active);
			ImGui::MenuItem("Configuration", NULL, &configuration->active);
			ImGui::MenuItem("Inspector", NULL, &inspector->active);
			ImGui::MenuItem("Hierarchy", NULL, &hierarchy->active);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Console"))
		{

			ImGui::MenuItem("Show Console", NULL, &console->active);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			HelpMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Close"))
		{
			if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				to_close = true;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();



		if (show_demo_window)				ImGui::ShowDemoWindow(&show_demo_window);
	
}

void ModuleImGui::ToggleEditor()
{
	EditorOff = !EditorOff;
}

void ModuleImGui::AddLog(const char* Log)
{
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


