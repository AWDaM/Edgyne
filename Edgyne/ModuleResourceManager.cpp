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

//uint ModuleResourceManager::ImportFile(const char * file, Resource::ResourceType type)
//{
//	std::string written_file;
//	std::vector<uint> meshesUID;
//	std::vector<uint> animationsUID;
//	std::vector<uint> bonesUID;
//	bool loaded = false;
//
//	bool newMeta = false;
//	JSON_File* meta = getMeta(file);
//	if (meta == nullptr)
//	{
//		meta = createMeta(file, type);
//		newMeta = true;
//	}
//	JSON_Value* metaValue = meta->getValue("meta");
//	uint UID = 0;
//	if (metaValue != nullptr)
//	{
//		UID = metaValue->getUint("UID");
//
//		if (newMeta || wasFileChanged(file))
//		{
//			switch (type)
//			{
//			case Resource::RES_MATERIAL:
//				//loaded = App->importer->importTexture(file, UID, written_file, metaValue);
//				break;
//				case  Resource::RESOURCE_MESH:
//				//loaded = App->scene_loader->importScene(file, UID, meshesUID, animationsUID, bonesUID, written_file, metaValue, newMeta);
//				//meta->setValue("meta", metaValue);
//				break;
//			}
//
//			if (loaded)
//			{
//				Resource* old = GetResource(UID);
//				uint timesLoaded = 0;
//				if (old != nullptr)
//				{
//					timesLoaded = old->GetTimesLoaded();
//					deleteResource(UID);
//				}
//				Resource* newRes = AddResource(type, UID);
//				std::string filename;
//				App->fileSystem->splitPath(file, nullptr, &filename, nullptr);
//				newRes->name = filename;
//				newRes->file = file;
//				newRes->exported_file = written_file;
//
//				if (type == R_SCENE)
//				{
//					((ResourceScene*)newRes)->meshesUID = meshesUID;
//					((ResourceScene*)newRes)->animationsUID = animationsUID;
//					((ResourceScene*)newRes)->bonesUID = bonesUID;
//				}
//
//				for (int i = 0; i < timesLoaded; i++)
//				{
//					newRes->LoadtoMemory(); //Load it for each time the existing resource was loaded
//				}
//				LOG("File imported");
//
//				if (newMeta)
//					meta->Write();
//				else //If meta is not new, means that the file has been modified, so update meta
//					updateMetaLastChange(file);
//			}
//		}
//		else if (GetResource(UID) == nullptr) //If the file is imported but the resources is not created
//		{
//			Resource* newRes = AddResource(type, UID);
//			std::string filename;
//			App->fileSystem->splitPath(file, nullptr, &filename, nullptr);
//			newRes->name = filename;
//			newRes->file = file;
//
//			switch (type)
//			{
//			case R_TEXTURE:
//				newRes->exported_file = TEXTURES_FOLDER + std::to_string(UID) + TEXTURES_EXTENSION;
//				break;
//
//			case R_SCENE:
//				JSON_Value* meshes = metaValue->getValue("meshes");
//				if (meshes != nullptr)
//				{
//					//Create the resource for each mesh inside the meta
//					for (rapidjson::Value::MemberIterator it_m = meshes->getRapidJSONValue()->MemberBegin(); it_m != meshes->getRapidJSONValue()->MemberEnd(); it_m++)
//					{
//						uint meshUID = (*it_m).value.GetUint();
//						Resource* meshResource = App->resources->AddResource(R_MESH, meshUID);
//						meshResource->name = (*it_m).name.GetString();
//						meshResource->file = file;
//						meshResource->exported_file = MESHES_FOLDER + std::to_string(meshUID) + MESH_EXTENSION;
//						((ResourceScene*)newRes)->meshesUID.push_back(meshUID); //Add its UID to the scene resource list
//					}
//				}
//				break;
//
//				/*case R_ANIMATION:
//					LOG("animation mesh");
//					break;*/
//			}
//
//			LOG("File was already imported, loading resource");
//			loaded = true;
//		}
//	}
//}

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

int ModuleResourceManager::getMetaLastChange(const char* path) const
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

	return mod_time > getMetaLastChange(file);
}
Resource* ModuleResourceManager::GetResourceFromUID(uint& UID)
{
	Resource* ret = nullptr;
	std::map<uint, Resource*>::iterator item = resources.find(UID);
	if (item != resources.end())
		return item->second;

	return ret;
}

Resource* ModuleResourceManager::CreateResource(Resource::ResourceType type)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case Resource::ResourceType::RES_MATERIAL:
		// ret = new ResourceMaterial();
		break;
	case Resource::ResourceType::RESOURCE_MESH:
		//ret = new ResourceMesh();
		break;
	}
	resources.emplace(ret->file, ret);
	return ret;
}

Resource * ModuleResourceManager::CreateNewResource(Resource::ResourceType type,std::string & file)
{
	Resource* ret = nullptr;
	switch (type)
	{
	case Resource::RES_MATERIAL:
		//ret = new ResourceMaterial(file);
		break;
	case Resource::RESOURCE_MESH:
		//ret = new ResourceMesh(file);
		break;
	}
	resources.emplace(ret->file, ret);
		return ret;
}


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


