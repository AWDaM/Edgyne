#include "Resource.h"

Resource::Resource(uint UID, ResourceType type) : uid(UID), type(type)
{}

Resource::Resource(uint UID, ResourceType type, const std::string & file) : uid(UID), type(type),file(file)
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
