#include"Application.h"
#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "TimeManager";
}


ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init(rapidjson::Value & node)
{
	return true;
}

bool ModuleTimeManager::CleanUp()
{
	return false;
}

update_status ModuleTimeManager::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::Begin("TimeManager");
	if (App->GAME_RUNNING)
	{
		if (ImGui::Button("Stop"))
			App->GAME_RUNNING = false;
	}
	else
		if (ImGui::Button("Play"))
			App->GAME_RUNNING = true;

	ImGui::End()
	return update_status();
}

void ModuleTimeManager::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleTimeManager::Load(rapidjson::Document & doc)
{
}
