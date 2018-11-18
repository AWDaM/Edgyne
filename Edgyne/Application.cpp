#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleLevel.h"
#include "ModuleDebug.h"
#include "ModuleLoader.h"
#include "ModuleImporter.h"
#include "ModuleTimeManager.h"
#include "ModuleResourceManager.h"

#include "p2Defs.h"

#include <time.h>

#include "rapidjson/filereadstream.h"

Application::Application() 
{
	PERF_START(ptimer);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imGui = new ModuleImGui(this);
	level = new ModuleLevel(this);
	debug = new ModuleDebug(this);
	loader = new ModuleLoader(this);
	importer = new ModuleImporter(this);
	time_manager = new ModuleTimeManager(this);
	resource_manager = new ModuleResourceManager(this);

	framerate = 1000 / 60;
	
	fps.resize(FPS_LOG_SIZE);

	ms.resize(FPS_LOG_SIZE);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);

	AddModule(input);
	AddModule(imGui);
	AddModule(level);
	AddModule(debug);
	AddModule(loader);
	AddModule(importer);
	AddModule(camera);
	AddModule(time_manager);
	AddModule(resource_manager);
	// Scenes

	// Renderer last!
	AddModule(renderer3D);
	PERF_PEEK(ptimer);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();
	while(item != list_modules.rend())
	{
		RELEASE(*item);
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
	rapidjson::Document document;

	FILE* fp = fopen("config.json", "rb");
	char readBuffer[65536];

	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	document.ParseStream(is);
	
	// Call Init() in all modules

	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(document[(*item)->name.data()]);
		item++;
	}
	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	 item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	game_dt = dt;
	if (game_state == RUNNING)
	{
		game_dt *= time_manager->time_scale;
	}
	else if (game_state == PAUSED)
	{
		game_dt *= 0;
	}
	frame_time.Start();
	ptimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.ReadMs() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.ReadMs();
	uint32 frames_on_last_update = prev_last_sec_frame_count;



	if (framerate > 0 && last_frame_ms < framerate) SDL_Delay(framerate - last_frame_ms);
	ChangeFPSlog(frames_on_last_update, last_frame_ms);
	
}

int Application::GetFramerate() const
{
	int ret;
	if (framerate > 0)
		ret =(int) ((1.0f / (float)framerate) * 1000.0f);
	else
		ret = 0;
	return ret;
}

void Application::SetFramerate(int _framerate)
{
	if (_framerate > 0)
		framerate = 1000 / _framerate;
	else
		framerate = 0;


}

void Application::ChangeFPSlog(int _fps, float _ms)
{
	
	
	for(int i = 0 ; i  < FPS_LOG_SIZE-1 ; i++)
	{
		fps[i] = fps[i+1];
	}
	fps[FPS_LOG_SIZE-1] = _fps;


	for (int i = 0; i < FPS_LOG_SIZE - 1; i++)
	{
		ms[i] = ms[i + 1];
	}
	ms[FPS_LOG_SIZE - 1] = _ms;
	
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{

	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->PreUpdate(game_dt);
		item++;
	}

	 item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Update(game_dt);
		item++;
	}

	 item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->PostUpdate(game_dt);
		item++;
	}

	if (toSave)
	{
		SaveData();
	}
	if (toLoad)
	{
		LoadData();
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	canLog = false;
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::OpenBrowser(std::string url)
{
	ShellExecuteA(NULL, "open", url.data(), NULL, NULL, SW_SHOWNORMAL);
}

void Application::Log(const char* entry)
{

	// save all logs, so we can dump all in a file upon close
	log.append(entry);
	if (canLog)
	{
	// send to editor console
	imGui->AddLog(log.data());
	}
}
void Application::Configuration_ImGui()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		static char name[25];
		strcpy_s(name, 25, GetTitle());
		if (ImGui::InputText("Application Name", name, 25, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			SetTitle(name);
		}
		int framerate_imgui = GetFramerate();
		if (ImGui::Checkbox("Vsync ON", &vsync_on))
		{
			if (vsync_on)
				SDL_GL_SetSwapInterval(1);
			else
				SDL_GL_SetSwapInterval(0);
		}

		if (ImGui::SliderInt("Max FPS", &framerate_imgui, 0, 120))
		{
			SetFramerate(framerate_imgui);
		}

		ImGui::Text("Framerate: %0.1f  ", fps[FPS_LOG_SIZE-1]);
		ImGui::SameLine();
		ImGui::PlotHistogram("", &fps[0], fps.size(), 0, "", 0.0f, 100.0f, ImVec2(310, 100));

		ImGui::Text("Miliseconds: %0.1f", ms[FPS_LOG_SIZE - 1]);
		ImGui::SameLine();
		ImGui::PlotHistogram("", &ms[0], ms.size(), 0, "", 0.0f, 10.0f, ImVec2(310, 100));
	}
}

void Application::Hardware_ImGui()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPU count: %i", SDL_GetCPUCount());
		ImGui::Text("Cache Size: %i kb", SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %i MB", SDL_GetSystemRAM());
		if (SDL_Has3DNow())
			ImGui::Text("CPU has 3DNow!");
		if (SDL_HasAVX())
			ImGui::Text("CPU has AVX");
		if (SDL_HasAVX2())
			ImGui::Text("CPU has AVX2");
		if (SDL_HasAltiVec())
			ImGui::Text("CPU has AltiVec");
		if (SDL_HasMMX())
			ImGui::Text("CPU has MMX");
		if (SDL_HasRDTSC())
			ImGui::Text("CPU has RDTSC");
		if (SDL_HasSSE())
			ImGui::Text("CPU has SSE");
		if (SDL_HasSSE2())
			ImGui::Text("CPU has SSE2");
		if (SDL_HasSSE3())
			ImGui::Text("CPU has SSE3");
		if (SDL_HasSSE41())
			ImGui::Text("CPU has SSE41");
		if (SDL_HasSSE42())
			ImGui::Text("CPU has SSE42");

	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

const char* Application::GetTitle() const
{
	return window_name;
}
void Application::SetTitle(char* title)
{
	window_name = title;
	SDL_SetWindowTitle(App->window->window, title);
}

void Application::SaveData()
{
	rapidjson::Document document;
	document.SetObject();
	FILE* fp = fopen("save.json", "wb");
	char writeBuffer[655360];

	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->Save(document, os);
	}

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);

	fclose(fp);

	toSave = false;
}

void Application::LoadData()
{
	rapidjson::Document saveFile;

	App->importer->LoadMeshFromFile();

	FILE* file = fopen("save.json", "rb");
	if (file)
	{
		char readBuffer[65536];

		rapidjson::FileReadStream inputStream(file, readBuffer, sizeof(readBuffer));

		saveFile.ParseStream(inputStream);

		std::list<Module*>::iterator item = list_modules.begin();

		while (item != list_modules.end())
		{
			(*item)->Load(saveFile);
			item++;
		}
	}
	else
	{
		LOG("Couldn't load dadta from save file");
	}
	toLoad = false;
}

float Application::GetDeltaTime()
{
	return dt;
}

uint64 Application::GetFrameCount()
{
	return frame_count;
}
