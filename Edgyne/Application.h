#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleTest.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "MathGeoLib\MathGeoLib.h"

#include <list>
#include <vector>
#include <string>
#include <queue>

class Application
{
public:
	ModuleWindow* window;
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
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};