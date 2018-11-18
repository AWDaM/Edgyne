#include "ModuleImporter.h"
#include "ModuleRenderer3D.h"
#include "ModuleLevel.h"
#include "ModuleResourceManager.h"
#include "Application.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include <experimental/filesystem>

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Importer";
}


ModuleImporter::~ModuleImporter()
{
}
bool ModuleImporter::Init(rapidjson::Value& node)
{
	LoadAllExtensionsAndPaths(node);

	return true;
}
bool ModuleImporter::CleanUp()
{
	RELEASE(meshExtension);
	RELEASE(meshLibraryPath);
	RELEASE(materialExtension);
	RELEASE(materialLibraryPath);
	RELEASE(sceneExtension);
	RELEASE(modelExtension);

	return true;
}
void ModuleImporter::LoadAllExtensionsAndPaths(rapidjson::Value & node)
{
	//Extensions
	meshExtension = new char[node["Mesh Extension"].GetStringLength()];
	strcpy(meshExtension, node["Mesh Extension"].GetString());

	materialExtension = new char[node["Material Extension"].GetStringLength()];
	strcpy(materialExtension, node["Material Extension"].GetString());

	//Paths
	meshLibraryPath = new char[node["Mesh Library Path"].GetStringLength()];
	strcpy(meshLibraryPath, node["Mesh Library Path"].GetString());

	materialLibraryPath = new char[node["Material Library Path"].GetStringLength()];
	strcpy(materialLibraryPath, node["Material Library Path"].GetString());

	sceneExtension = new char[node["Scene Extension"].GetStringLength()];
	strcpy(sceneExtension, node["Scene Extension"].GetString());

	modelExtension = new char[node["Model Extension"].GetStringLength()];
	strcpy(modelExtension, node["Model Extension"].GetString());
}


void ModuleImporter::SaveTexture(std::string& path)
{
	std::string fileName = path;
	fileName = fileName.erase(0, fileName.find_last_of("\\")+1);
	fileName = fileName.substr(0, fileName.find("."));


	FILE* file = fopen(path.c_str(), "rb");

	char* buffer = nullptr;
	int size = 0;
	if (file != NULL)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);
		buffer = new char[size];
		fread(buffer, 1, size, file);
	}

	ILuint img = ilGenImage();
	ilBindImage(img);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		ILuint ilSize;
		ILubyte* data;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

		ilSize = ilSaveL(IL_DDS, NULL, 0);
		if (ilSize > 0)
		{
			data = new ILubyte[ilSize];
			if (ilSaveL(IL_DDS, data, ilSize))
			{
				std::string currMaterialPath = materialLibraryPath;
				currMaterialPath.append(fileName);
				currMaterialPath.append(materialExtension);


				FILE* file = fopen(currMaterialPath.data(), "wb");
				fwrite(data, sizeof(char), ilSize, file);
				fclose(file);
			}
		}
	}
}

bool ModuleImporter::LoadMeshFromFile()
{
	const std::experimental::filesystem::directory_iterator end{};

	for (std::experimental::filesystem::directory_iterator iter{ meshLibraryPath }; iter != end; ++iter)
	{
		if (std::experimental::filesystem::is_regular_file(*iter))
			CopyDataFromFile(iter->path().string());
	}

	return true;
}

void ModuleImporter::CopyDataFromFile(std::string& path, ResourceMesh* _mesh)
{
	if (path != "No File")
	{
		FILE* file = fopen(path.data(), "rb");
		char* buffer;
		if (file != NULL)
		{
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			rewind(file);
			buffer = new char[size];
			fread(buffer, 1, size, file);
		}
		char* bookmark = buffer;

		uint ranges[2];
		bool optatives[3];
		uint bytes = sizeof(ranges);

		memcpy(ranges, bookmark, bytes);
		bookmark += bytes;

		bytes = sizeof(optatives);
		memcpy(optatives, bookmark, bytes);
		bookmark += bytes;

		//if (!_mesh)
		//{
		//	path = path.erase(0, sizeof(meshLibraryPath) - 1);
		//	GameObject* game_object = App->level->NewGameObject((char*)path.erase(path.find_last_of("."), sizeof(meshExtension)).data());

		//	_mesh = (Mesh*)game_object->AddComponent(MESH);
		//}

		_mesh->num_vertex = ranges[0];
		_mesh->num_index = ranges[1];
		_mesh->has_texture_coordinates = optatives[0];
		_mesh->has_triangle_faces = optatives[1];
		_mesh->has_normals = optatives[2];

		bytes = sizeof(float)*_mesh->num_vertex * 3;

		_mesh->vertex = new float[_mesh->num_vertex * 3];
		memcpy(_mesh->vertex, bookmark, bytes);

		bookmark += bytes;
		bytes = sizeof(uint)*_mesh->num_index;

		_mesh->index = new uint[_mesh->num_index];
		memcpy(_mesh->index, bookmark, bytes);

		bookmark += bytes;

		if (_mesh->has_texture_coordinates)
		{
			bytes = sizeof(float)*_mesh->num_vertex * 2;

			_mesh->texCoords = new float[_mesh->num_vertex * 2];
			memcpy(_mesh->texCoords, bookmark, bytes);

			bookmark += bytes;
		}
		if (_mesh->has_normals)
		{
			bytes = sizeof(float)*_mesh->num_vertex * 3;

			_mesh->normals = new float[_mesh->num_vertex * 3];
			memcpy(_mesh->normals, bookmark, bytes);
		}
		//_mesh->SetBoundingVolume();
		fclose(file);


		glGenBuffers(1, (GLuint*)&(_mesh->id_index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * _mesh->num_index, &_mesh->index[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


std::string ModuleImporter::FindFileInFolder(std::string& fileName)
{
	std::string ret = "No File";

	char* file = new char();
	strcat(file, meshLibraryPath);
	strcat(file, fileName.append(".edgymesh").c_str());

	if(std::experimental::filesystem::exists(file));
		ret = file;

	return ret;
}

const char* ModuleImporter::LoadDataFromFile(const char* file, uint& size)
{
	FILE* fp = fopen(file, "rb");
	char* buffer;
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		rewind(fp);
		buffer = new char[size];
		fread(buffer, 1, size, fp);
	}
	fclose(fp);
	return buffer;
}

void ModuleImporter::WriteDataOnFile(const void* data, uint size, const char* file)
{
	FILE* fp = fopen(file, "wb");
	if (fp)
	{
		fwrite(data, sizeof(char), size, fp);
		fclose(fp);
	}
}





void ModuleImporter::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleImporter::Load(rapidjson::Document& doc)
{
}