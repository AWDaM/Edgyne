#pragma once

#include "Globals.h"
#include "Timer.h"
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

private:

	Timer	ms_timer;
	float	dt;


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void OpenBrowser(std::string);
	void Log(const char* entry);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	std::string log;

public:

	bool canLog = false;

};

extern Application* App;