#pragma once
#include "Module.h"
class ModuleLevel :
	public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();


	void Draw();
};

