#include"Application.h"
#include "ModuleTimeManager.h"



ModuleTimeManager::ModuleTimeManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "TimeManager";
	game_clock.Start();
	game_clock.PauseTimer();
	real_clock.Start();
	real_clock.PauseTimer();
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

	if (App->game_state == RUNNING)
	{
		game_clock_seconds += game_clock.ReadSec() * time_scale;
		game_clock.Start();
	}

	ImGui::Begin("TimeManager");

	if (ImGui::Button("Play"))
	{
		if (App->game_state == STOPPED)
		{
			game_clock.Start();
			game_clock_seconds = 0;
			real_clock.Start();
		}

		if (App->game_state == PAUSED)
		{
			game_clock.PauseTimer();
			real_clock.PauseTimer();
		}
		App->game_state = RUNNING;
	}

	ImGui::SameLine();

	if (ImGui::Button("Pause"))
		App->game_state = PAUSED;

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
		App->game_state = STOPPED;

	ImGui::SameLine();

	if (ImGui::Button("Next Frame"))
	{ }

	ImGui::SliderFloat("Time Scale", &time_scale, 0, 2);

	if (ImGui::Button("Reset Time Scale"))
		time_scale = 1.0f;

	float real_dt = App->GetDeltaTime();
	float game_dt = real_dt*time_scale;

	ImGui::Text("Game DeltaTime: %f",game_dt);
	ImGui::SameLine();
	ImGui::Text("Game Timer: %f", game_clock_seconds);

	ImGui::Text("Real DeltaTime: %f", real_dt);
	ImGui::SameLine();
	ImGui::Text("Real Timer: %f", real_clock.ReadSec());

	ImGui::End();
	return ret;
}

void ModuleTimeManager::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleTimeManager::Load(rapidjson::Document & doc)
{
}
