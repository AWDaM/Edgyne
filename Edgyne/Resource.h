#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

#include "rapidjson/document.h"// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"
#include "rapidjson\allocators.h"
#include "MathGeoLib/MathGeoLib.h"



class Resource
{
	friend class ModuleResourceManager;
public:
	enum ResourceType
	{
		UNKNOWN,
		RES_MATERIAL,
		RESOURCE_MESH,
		RES_SHADER
	};


	Resource(ResourceType type);
	Resource(ResourceType type, const std::string& file);
	virtual ~Resource();

	bool LoadResource();

	virtual bool LoadResourceToMemory();

public:
	std::string file;

	ResourceType type = UNKNOWN;
	uint amount_loaded = 0;
};

#endif // !__RESOURCE_H__