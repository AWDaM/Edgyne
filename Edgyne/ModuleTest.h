#pragma once
#include "Module.h"
#include "MathGeoLib\Geometry\Sphere.h" // this is shit
#include "MathGeoLib\Geometry\Frustum.h" // this is shit


class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool CreateSphere(float x, float y, float z, float r);
	bool CreateFrustum(float x, float y, float z, float r);
	bool CreateCylinder();
	bool CreateCapsule(float xt, float yt, float zt, float xb, float yb, float zb,float r);

	update_status Update(float dt);

	Sphere* sphere = nullptr;
	Frustum* frustum = nullptr;
	Cylinder* cylinder = nullptr;
	Capsule* capsule = nullptr;
	

};

