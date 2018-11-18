#include "Resource.h"

Resource::Resource(ResourceType type) : type(type)
{}

Resource::Resource(ResourceType type, const std::string & file) : type(type),file(file)
{
}

Resource::~Resource()
{
}

bool Resource::LoadResource()
{
	bool ret = true;
	if (amount_loaded == 0)
	{
		ret = LoadResourceToMemory();
		amount_loaded++;
	}
	else
		amount_loaded++;

	return ret;
}

bool Resource::LoadResourceToMemory()
{
	return true;
}
