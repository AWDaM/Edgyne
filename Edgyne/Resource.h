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
		RESOURCE_MESH
	};


	Resource(uint UID, ResourceType type);

	ResourceType GetType()const;
	uint GetUID()const;
	const char* GetName() const;
	const char* GetFile()const;
	const char* GetExportedFile()const;
	bool IsLoaded()const;
	//You should call this function
	bool LoadtoMemory();
	//You should call this function
	bool UnloadMemory();
	uint GetTimesLoaded() const;

	virtual bool LoadInMemory() { return false; }
	virtual bool UnloadFromMemory() { return false; }

public:
	std::string name = "";
	uint UID = 0;
	uint loaded = 0;
	std::string file = "";
	std::string exported_file = "";
	ResourceType type = UNKNOWN;

};

#endif // !__RESOURCE_H__