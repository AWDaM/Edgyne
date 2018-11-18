#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

#include "rapidjson/document.h"// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"
#include "rapidjson\allocators.h"
#include "MathGeoLib/MathGeoLib.h"

enum ResourceType
{
	UNKNOWN,
	MATERIAL,
	MESH
};

class Resource
{
	friend class ModuleResourceManager;
public:
	Resource(uint uid, ResourceType type);
	virtual ~Resource();

	bool LoadResource();

	virtual bool LoadResourceToMemory();

public:
	uint uid = 0;
	std::string file;
	std::string exported_file;

	ResourceType type = UNKNOWN;
	uint amount_loaded = 0;
};

#endif // !__RESOURCE_H__