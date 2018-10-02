#include "Application.h"
#include "ModuleLoader.h"



ModuleLoader::ModuleLoader(Application * app, bool start_enabled) : Module(start_enabled)
{
	name = "loader";
}

ModuleLoader::~ModuleLoader()
{
}
