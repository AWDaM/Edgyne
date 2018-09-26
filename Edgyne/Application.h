#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "MathGeoLib\MathGeoLib.h"

#include <list>
#include <vector>
#include <string>
#include <queue>

class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleImGui;
class ModuleTest;

class Application
{
public:
	ModuleWindow * window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleImGui* imGui;
	ModuleTest* test;

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
	char* window_name = TITLE;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void OpenBrowser(std::string);

	void Log(const char* entry);
	void Configuration_ImGui();
	void Hardware_ImGui();
	char* Application::GetTitle() const;
	void Application::SetTitle( char* title);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();


public:

	std::string log;
	bool canLog = false;
	int GetFramerate() const;
	void SetFramerate(int _framerate);
	void ChangeFPSlog(int _fps, float _ms);
};

extern Application* App;