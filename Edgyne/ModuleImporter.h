#ifndef __MODULE_FILE_SYSTEM__
#define __MODULE_FILE_SYSTEM__

#include "Module.h"
#include "Globals.h"

class Mesh;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(rapidjson::Value& node);
	void LoadAllExtensionsAndPaths(rapidjson::Value& node);

	bool SaveToFile(Mesh* mesh);
	void SaveTexture(std::string& path);

	bool LoadMeshFromFile();
	void CopyDataFromFile(std::string& path);


	const char* LoadDataFromFile(const char* file, uint& size); 
	void WriteDataOnFile(const void* data, uint size, const char* file);




	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);
public:
	char* meshExtension = nullptr;
	char* meshLibraryPath = nullptr;
	char* materialExtension = nullptr;
	char* materialLibraryPath = nullptr;
};

#endif // !__MODULE_FILE_SYSTEM__