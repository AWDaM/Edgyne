#include "Application.h"
#include "ModuleTest.h"
#include "p2Defs.h"




ModuleTest::ModuleTest(Application* app, bool start_enabled ) : Module(app, start_enabled)
{
	name = "Test";
}


ModuleTest::~ModuleTest()
{
}

bool ModuleTest::CreateSphere(float x, float y, float z, float r)
{
	
	RELEASE(sphere);
	math::vec vector = math::vec(x,y,z);
	sphere = new math::Sphere(vector, r);
	LOG("we've created an sphere with: radius = %f, x = %f, y = %f, z= %f", sphere->Diameter() / 2.0f, sphere->pos.x, sphere->pos.y, sphere->pos.z);
	return true;
}

bool ModuleTest::CreateFrustum(float x, float y, float z, float r)
{
	//RELEASE(frustum);
	//math::vec vector = math::vec(x, y, z);
	//frustum = new math::Frustum(x,)
	return false;
}

bool ModuleTest::CreateCylinder()
{
	math::vec test = math::vec(5, 5, 5);
	//cylinder = new math::Cylinder(test);
	return true;
}

bool ModuleTest::CreateCapsule(float xt, float yt, float zt, float xb, float yb, float zb, float r)
{
	RELEASE(capsule);
	math::vec top = math::vec(xt,yt,zt);
	math::vec bottom = math::vec(xb,yb,zb);
	capsule = new math::Capsule(bottom, top, r);
	return true;
}

bool ModuleTest::CreateAABB(float maxx, float maxy, float maxz, float minx, float miny, float minz)
{
	RELEASE(aabb);
	math::vec max = math::vec(maxx,maxy,maxz);
	math::vec min = math::vec(minx,miny,minz);
	aabb = new math::AABB(min, max);
	return true;
}

bool ModuleTest::CheckTestIntersections()
{
	if (sphere != nullptr && capsule != nullptr && aabb != nullptr)
	{
		if (sphere->Intersects(*capsule) || sphere->Intersects(*aabb))
		{
			sphere_intersection = true;
		}
		else
		{
			sphere_intersection = false;
		}

		if (capsule->Intersects(*aabb) || capsule->Intersects(*sphere))
		{
			capsule_intersection = true;
		}
		else
		{
			capsule_intersection = false;
		}

		if (aabb->Intersects(*sphere) || aabb->Intersects(*capsule))
		{
			aabb_intersection = true;
		}
		else
		{
			aabb_intersection = false;
		}
	}
	return true;
}





update_status ModuleTest::Update(float dt)
{

	//limonera->pos.x += 0.1;

	//
	//if (pera->Intersects(*limonera))
	//{
	//	LOG("IT WORKS IT WORKS");
	//}
	//
	CheckTestIntersections();
	return UPDATE_CONTINUE;
}

