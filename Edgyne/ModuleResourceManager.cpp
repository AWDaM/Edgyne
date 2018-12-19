#include "ModuleResourceManager.h"
#include "Application.h"
#include "Material.h"
#include "Mesh.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "ResourceManager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}

uint ModuleResourceManager::ImportFile(const char* file, Resource::ResourceType type)
{
	std::string written_file;
	std::vector<uint> meshesUID;

	bool loaded = false;

	bool newMeta = false;
	JSON_File* meta = getMeta(file);
	if (meta == nullptr)
	{
		meta = createMeta(file, type);
		newMeta = true;
	}
	JSON_Value* metaValue = meta->getValue("meta");
	uint UID = 0;
	if (metaValue != nullptr)
	{
		UID = metaValue->getUint("UID");

		if (newMeta || wasFileChanged(file))
		{
			switch (type)
			{
			case Resource::RES_MATERIAL:
				//loaded = App->importer->importTexture(file, UID, written_file, metaValue);
				break;
				case  Resource::RESOURCE_MESH:
				//loaded = App->scene_loader->importScene(file, UID, meshesUID, animationsUID, bonesUID, written_file, metaValue, newMeta);
				//meta->setValue("meta", metaValue);
				break;
			}

			if (loaded)
			{
				Resource* old = GetResourceFromUID(UID);
				uint timesLoaded = 0;
				if (old != nullptr)
				{
					timesLoaded = old->GetTimesLoaded();
					DeleteResource(UID);
				}
				Resource* newRes = AddResource(UID, type);
				std::string fileName;

				fileName = fileName.erase(0, fileName.find_last_of("\\") + 1);
				fileName = fileName.substr(0, fileName.find("."));
				newRes->name = fileName;
				newRes->file = file;
				newRes->exported_file = written_file;

				//if (type == R_SCENE)
				//{
				//	((ResourceScene*)newRes)->meshesUID = meshesUID;
				//	((ResourceScene*)newRes)->animationsUID = animationsUID;
				//	((ResourceScene*)newRes)->bonesUID = bonesUID;
				//}

				for (int i = 0; i < timesLoaded; i++)
				{
					newRes->LoadtoMemory(); //Load it for each time the existing resource was loaded
				}
				LOG("File imported");

				if (newMeta)
					meta->Write();
				else //If meta is not new, means that the file has been modified, so update meta
					UpdateMetaLastChange(file);
			}
		}
		else if (GetResourceFromUID(UID) == nullptr) //If the file is imported but the resources is not created
		{
			std::string _file;

			Resource* newRes = AddResource(UID, type);
			
			std::string filename;

			switch (type)
			{
			case Resource::RES_MATERIAL:
				newRes->exported_file = "Library\\Materials\\" + std::to_string(UID) + ".dds";
				break;

			case Resource::RESOURCE_MESH:
				JSON_Value* meshes = metaValue->getValue("meshes");
				if (meshes != nullptr)
				{
					//Create the resource for each mesh inside the meta
					for (rapidjson::Value::MemberIterator it_m = meshes->getRapidJSONValue()->MemberBegin(); it_m != meshes->getRapidJSONValue()->MemberEnd(); it_m++)
					{
						uint meshUID = (*it_m).value.GetUint();
						Resource* meshResource = App->resource_manager->AddResource(meshUID, Resource::RESOURCE_MESH);
						meshResource->name = (*it_m).name.GetString();
						meshResource->file = file;
						meshResource->exported_file = "Library\\Meshes\\" + std::to_string(meshUID) + ".edgymesh";
						//((ResourceScene*)newRes)->meshesUID.push_back(meshUID); //Add its UID to the scene resource list
					}
				}
				break;

				/*case R_ANIMATION:
					LOG("animation mesh");
					break;*/
			}

			LOG("File was already imported, loading resource");
			loaded = true;
		}
	}
}

JSON_File* ModuleResourceManager::getMeta(const char * path) const
{
	std::string metaPath = path;
	metaPath += ".meta";
	JSON_File* ret = App->JSON_manager->openReadFile(metaPath.c_str());

	return ret;
}

JSON_File* ModuleResourceManager::createMeta(const char* path, Resource::ResourceType type) const
{
	std::string metaPath = path;
	metaPath += ".meta";
	JSON_File* ret = App->JSON_manager->openWriteFile(metaPath.c_str());

	JSON_Value* meta = ret->createValue();
	meta->addUint("UID", pcg32_random_r(&App->rng));

	struct stat result;
	if (stat(path, &result) == 0)
	{
		int mod_time = result.st_mtime; 
		meta->addInt("last_change", mod_time);
	}

	JSON_Value* importSettings = meta->createValue();

	meta->addValue("ImportSettings", importSettings);
	ret->addValue("meta", meta);

	return ret;
}

int ModuleResourceManager::GetMetaLastChange(const char* path) const
{
	int ret = 0;
	std::string metaPath = path;
	metaPath += ".meta";
	JSON_File* meta = App->JSON_manager->openReadFile(metaPath.c_str());
	if (meta != nullptr)
	{
		JSON_Value* metaValue = meta->getValue("meta");
		if (metaValue != nullptr)
			ret = metaValue->getInt("last_change");
	}

	App->JSON_manager->closeFile(meta);
	return ret;
}
bool ModuleResourceManager::wasFileChanged(const char * file) const
{
	int mod_time;
	struct stat result;
	if (stat(file, &result) == 0)
		mod_time = result.st_mtime;

	return mod_time > GetMetaLastChange(file);
}
Resource* ModuleResourceManager::GetResourceFromUID(uint& UID)
{
	Resource* ret = nullptr;
	std::map<uint, Resource*>::iterator item = resources.find(UID);
	if (item != resources.end())
		return item->second;

	return ret;
}

Resource* ModuleResourceManager::AddResource(uint& UID, Resource::ResourceType type)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case Resource::ResourceType::RES_MATERIAL:
		ret = new ResourceMaterial(UID, type);
		break;
	case Resource::ResourceType::RESOURCE_MESH:
		ret = new ResourceMesh(UID, type);
		break;
	}
	resources.emplace(ret->file, ret);
	return ret;
}

bool ModuleResourceManager::DeleteResource(uint uid)
{
	if (resources.find(uid) != resources.end())
	{
		Resource* res = resources[uid];
		resources.erase(uid);
		RELEASE(res);

		return true;
	}

	return false;
}

bool ModuleResourceManager::UpdateMetaLastChange(const char * path)
{
	bool ret = false;
	std::string metaPath = path;
	metaPath += ".meta";
	JSON_File* readFile = App->JSON_manager->openReadFile(metaPath.c_str());
	if (readFile != nullptr)
	{
		JSON_File* writeFile = App->JSON_manager->openWriteFile(metaPath.c_str());
		JSON_Value* meta = readFile->getValue("meta");
		if (meta != nullptr)
		{
			struct stat result;
			if (stat(path, &result) == 0)
			{
				meta->setUint("last_change", result.st_mtime);
			}
			
			writeFile->addValue("meta", meta);

			writeFile->Write();
		}
		App->JSON_manager->closeFile(writeFile);
		ret = true;
	}
	App->JSON_manager->closeFile(readFile);
	return ret;
}

//Resource* ModuleResourceManager::CreateNewResource(uint& UID, Resource::ResourceType type,std::string & file)
//{
//	Resource* ret = nullptr;
//	switch (type)
//	{
//	case Resource::RES_MATERIAL:
//		ret = new ResourceMaterial(UID, file);
//		break;
//	case Resource::RESOURCE_MESH:
//		ret = new ResourceMesh(UID, file);
//		break;
//	}
//	resources.emplace(ret->file, ret);
//		return ret;
//}


//Resource* ModuleResourceManager::CreateNewResource(Resource::ResourceType type, const std::string& file)
//{
//	Resource* ret = nullptr;
//	switch (type)
//	{
//	case Resource::RES_MATERIAL:
//		ret = new ResourceMaterial(file);
//		break;
//	case Resource::RESOURCE_MESH:
//		ret = new ResourceMesh(file);
//		break;
//		resources.emplace(ret->file, ret);
//		return ret;
//	}
//}


