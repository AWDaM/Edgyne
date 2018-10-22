#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include <experimental/filesystem>

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "FileSystem";
}


ModuleFileSystem::~ModuleFileSystem()
{
}
bool ModuleFileSystem::Init(rapidjson::Value& node)
{
	return true;
}


// Method to save on our own file format. Header reads in order: Vertex - Index - Tex Coords - Normals 
bool ModuleFileSystem::SaveToFile(mesh* mesh)
{
	uint ranges[2] = { mesh->num_vertex, mesh->num_index };

//					RANGES OF DATA					VERTICES							INDICES							TEX COORDS							NORMALS
	uint fileSize = sizeof(ranges) + sizeof(float)*mesh->num_vertex * 3 + sizeof(uint)*mesh->num_index + sizeof(float)*mesh->num_vertex * 2 + sizeof(float)*mesh->num_vertex * 3;

	char* data = new char[fileSize];
	char* bookmark = data;

	// Saving the ranges of each of the components
	uint bytes = sizeof(ranges);
	memcpy(bookmark, ranges, bytes);

	bookmark += bytes;

	// Saving the data of the vertices
	bytes = sizeof(float)*mesh->num_vertex * 3;
	memcpy(bookmark, mesh->vertex, bytes);

	bookmark += bytes;

	// Saving the data of the indices
	bytes = sizeof(uint)*mesh->num_index;
	memcpy(bookmark, mesh->index, bytes);

	bookmark += bytes;

	// Saving the data of the texture coordinates
	bytes = sizeof(float)*mesh->num_vertex * 2;
	memcpy(bookmark, mesh->texCoords, bytes);

	bookmark += bytes;

	// Saving the data of the normals
	bytes = sizeof(float)*mesh->num_vertex * 3;
	memcpy(bookmark, mesh->normals, bytes);

	std::string str = "Library\\";
	str.append(mesh->name);
	str.append(".edgy");

	FILE* fp = fopen(str.data(), "wb");
	fwrite(data, sizeof(char), fileSize, fp);
	fclose(fp);

	return true;
}

bool ModuleFileSystem::LoadFromFile()
{
	std::vector<std::string> filenames;
	std::string str = App->projectFile;
	str.append("Library\\");

	const std::experimental::filesystem::directory_iterator end{};

	for (std::experimental::filesystem::directory_iterator iter{ str.data() }; iter != end; ++iter)
	{
		if (std::experimental::filesystem::is_regular_file(*iter))
			filenames.push_back(iter->path().string());
	}






	return true;
}



void ModuleFileSystem::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleFileSystem::Load(rapidjson::Document& doc)
{
}