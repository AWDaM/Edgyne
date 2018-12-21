#include "Resource.h"

Resource::Resource(ResourceType type) : type(type)
{}

Resource::Resource(uint& uid, ResourceType type, const std::string & file) : uid(uid),type(type),file(file)
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
