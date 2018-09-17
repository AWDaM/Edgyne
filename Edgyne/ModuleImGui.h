#pragma once
#include "Module.h"
class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
private:
public:
	bool show_demo_window = false;
	bool to_close = false;
};

