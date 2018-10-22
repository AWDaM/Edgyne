#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "MathGeoLib\MathGeoLib.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

#include <list>
#include <vector>
#include <string>
#include <queue>

class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleImGui;
class ModuleLevel;
class ModuleDebug;
class ModuleLoader;
class ModuleFileSystem;

class Application
{
public:
	ModuleWindow * window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleImGui* imGui;
	ModuleLevel* level;
	ModuleDebug* debug;
	ModuleLoader* loader;
	ModuleFileSystem* fileSystem;

	std::list<Module*> list_modules;

	int			framerate = 0;

private:

	Timer				ms_timer;
	PerfTimer			ptimer;
	uint64				frame_count = 0;
	PerfTimer			startup_time;
	PerfTimer			frame_time;
	PerfTimer			last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float	dt;

	std::vector<float> fps;
	std::vector<float> ms;

public:

	Application(char* projectFile);
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void OpenBrowser(std::string);

	void Log(const char* entry);
	void Configuration_ImGui();
	void Hardware_ImGui();
	const char* Application::GetTitle() const;
	void Application::SetTitle( char* title);

	void SaveData();
	void LoadData();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();


public:
	char* projectFile;
	const char* window_name = nullptr;
	bool vsync_on = true;
	bool toSave = false;
	bool toLoad = false;
	std::string log;
	bool canLog = false;
	int GetFramerate() const;
	void SetFramerate(int _framerate);
	void ChangeFPSlog(int _fps, float _ms);
};

extern Application* App;