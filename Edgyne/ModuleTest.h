#pragma once
#include "Module.h"
#include "MathGeoLib\Geometry\Sphere.h" // this is shit




class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool CreateSphere(float x, float y, float z, float r);
	bool CreateCylinder();

	update_status Update(float dt);

	Sphere* sphere = nullptr;
	Cylinder* cylinder = nullptr;
	

};

