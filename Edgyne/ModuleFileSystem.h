#ifndef __MODULE_FILE_SYSTEM__
#define __MODULE_FILE_SYSTEM__




#include "Module.h"
#include "Globals.h"

struct mesh;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init(rapidjson::Value& node);

	bool SaveToFile(mesh* mesh);
	void SaveTexture(std::string& path, std::string& fileName);

	bool LoadFromFile();

	void CopyDataFromFile(std::string& path);


	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);
public:
	char* fileExtension = ".edgy";
	char* libraryPath = "Library\\";
};

#endif // !__MODULE_FILE_SYSTEM__