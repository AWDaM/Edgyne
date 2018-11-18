#ifndef __MODULE_FILE_SYSTEM__
#define __MODULE_FILE_SYSTEM__

#include "Module.h"
#include "Globals.h"

class Mesh;
class ResourceMesh;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(rapidjson::Value& node);
	bool CleanUp();
	void LoadAllExtensionsAndPaths(rapidjson::Value& node);

	bool SaveToFile(Mesh* mesh);
	void SaveTexture(std::string& path);

	bool LoadMeshFromFile();
	void CopyDataFromFile(std::string& path, ResourceMesh* _mesh = nullptr);
	std::string FindFileInFolder(std::string& fileName);

	const char* LoadDataFromFile(const char* file, uint& size); 
	void WriteDataOnFile(const void* data, uint size, const char* file);




	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);
public:
	char* meshExtension = nullptr;
	char* meshLibraryPath = nullptr;
	char* materialExtension = nullptr;
	char* materialLibraryPath = nullptr;
	char* sceneExtension = nullptr;
	char* modelExtension = nullptr;
};

#endif // !__MODULE_FILE_SYSTEM__