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

	float real_dt = 0.0f;
	float game_dt = 0.0f;

	if (App->game_state == RUNNING || App->game_state == ONE_FRAME)
	{
		frame_count++;
		game_clock_seconds += game_clock.ReadSec() * time_scale;
		game_clock.Start();
		 real_dt = App->GetDeltaTime();
		 game_dt = real_dt * time_scale;
		 if (App->game_state == ONE_FRAME)
		 {
			 App->game_state = PAUSED;
			 real_clock.PauseTimer();
			 game_clock.PauseTimer();
		 }
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
			game_clock.Start();
			real_clock.Start();
		}
		App->game_state = RUNNING;
	}

	ImGui::SameLine();

	if (ImGui::Button("Pause"))
	{
		App->game_state = PAUSED;
		game_clock.PauseTimer();
		real_clock.PauseTimer();
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		App->game_state = STOPPED;
		game_clock_seconds = 0;
		game_clock.Start();
		game_clock.PauseTimer();
		real_clock.Start();
		real_clock.PauseTimer();
		frame_count = 0;
	}

	ImGui::SameLine();

	if (ImGui::Button("Next Frame"))
	{
		App->game_state = ONE_FRAME;
		real_clock.Start();
		game_clock.Start();
	}

	ImGui::SliderFloat("Time Scale", &time_scale, 0, 2);

	if (ImGui::Button("Reset Time Scale"))
		time_scale = 1.0f;



	if (App->game_state == PAUSED)
	{
		real_dt = 0;
		game_dt = 0;
	}

	ImGui::Text("Game DeltaTime: %f",game_dt);
	ImGui::SameLine();
	ImGui::Text("Game Timer: %f", game_clock_seconds);

	ImGui::SameLine();
	ImGui::Text("Frame Count: %i", frame_count);

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
