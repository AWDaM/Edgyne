#include "Application.h"
#include "ModuleTest.h"





ModuleTest::ModuleTest(Application* app, bool start_enabled ) : Module(app, start_enabled)
{
	//math::vec test = math::vec(5, 5, 5);

	//math::vec test2 = math::vec(-10, 5, 5);

	//pera = new math::Sphere(test, 6.0);
	//limonera = new math::Sphere(test2, 4.0);
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





update_status ModuleTest::Update(float dt)
{

	//limonera->pos.x += 0.1;

	//
	//if (pera->Intersects(*limonera))
	//{
	//	LOG("IT WORKS IT WORKS");
	//}
	//

	return UPDATE_CONTINUE;
}

