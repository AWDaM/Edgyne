#include "Application.h"
#include "ModuleTest.h"





ModuleTest::ModuleTest(Application* app, bool start_enabled ) : Module(app, start_enabled)
{
}


ModuleTest::~ModuleTest()
{
}





update_status ModuleTest::Update(float dt)
{

	math::vec test = math::vec(5, 5, 5);

	math::Sphere pera =  math::Sphere(test, 6.0);
	math::Sphere limonera = math::Sphere(test, 4.0);
	if (pera.Intersects(limonera))
	{
		LOG("IT WORKS IT WORKS");
	}
	

	return UPDATE_CONTINUE;
}

