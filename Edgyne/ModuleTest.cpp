#include "Application.h"
#include "ModuleTest.h"





ModuleTest::ModuleTest(Application* app, bool start_enabled ) : Module(app, start_enabled)
{
	math::vec test = math::vec(5, 5, 5);

	math::vec test2 = math::vec(-10, 5, 5);

	pera = new math::Sphere(test, 6.0);
	limonera = new math::Sphere(test2, 4.0);
}


ModuleTest::~ModuleTest()
{
}





update_status ModuleTest::Update(float dt)
{

	limonera->pos.x += 0.1;

	
	if (pera->Intersects(*limonera))
	{
		LOG("IT WORKS IT WORKS");
	}
	

	return UPDATE_CONTINUE;
}

