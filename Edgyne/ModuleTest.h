#pragma once
#include "Module.h"
#include "MathGeoLib\Geometry\Sphere.h" // this is shit
#include "MathGeoLib\Geometry\Frustum.h" // this is shit

#include <string>

class ModuleTest : public Module
{
public:
	ModuleTest(Application* app, bool start_enabled = true);
	~ModuleTest();

	bool CreateSphere(float x, float y, float z, float r);
	bool CreateFrustum(float x, float y, float z, float r);
	bool CreateCylinder();
	bool CreateCapsule(float xt, float yt, float zt, float xb, float yb, float zb,float r);
	bool CreateAABB(float maxx, float maxy, float maxz, float minx, float miny, float minz);

	bool CheckTestIntersections();

	bool Init();

	update_status Update(float dt);

	Sphere* sphere = nullptr;

	bool sphere_intersection = false;
	Cylinder* cylinder = nullptr;
	bool cylinder_intersection = false;
	Capsule* capsule = nullptr;
	bool capsule_intersection = false;
	AABB* aabb = nullptr;
	bool aabb_intersection = false;
	Frustum* frustum = nullptr;

	const char* json;
	int num;
	std::string string;
};

