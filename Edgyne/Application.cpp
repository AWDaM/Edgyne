#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleTest.h"
#include "p2Defs.h"

Application::Application()
{


	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imGui = new ModuleImGui(this);
	test = new ModuleTest(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(test);
	AddModule(imGui);

	// Scenes

	// Renderer last!
	AddModule(renderer3D);
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

	// Call Init() in all modules

	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
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
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{

	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	 item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	 item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
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
		ret = (*item)->Init();
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
	if (canLog)
	{
		// save all logs, so we can dump all in a file upon close
		log.append(entry);

		// send to editor console
		imGui->AddLog(entry);
	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}