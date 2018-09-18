#pragma once
#include "Module.h"


class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();



	update_status Update(float dt);


};

