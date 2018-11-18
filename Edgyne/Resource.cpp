#include "Resource.h"

Resource::Resource(uint uid, ResourceType type) : uid(uid),type(type)
{}

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
