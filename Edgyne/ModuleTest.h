#pragma once
#include "Module.h"
#include "MathGeoLib\Geometry\Sphere.h" // this is shit




class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();



	update_status Update(float dt);

	Sphere* pera;
	Sphere* limonera;
};

