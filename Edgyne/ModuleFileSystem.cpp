#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include <experimental/filesystem>

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")

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

	std::string str = "Library\\Meshes\\";
	str.append(mesh->name);
	str.append(".edgy");

	FILE* file = fopen(str.data(), "wb");
	fwrite(data, sizeof(char), fileSize, file);
	fclose(file);



	return true;
}

void ModuleFileSystem::SaveTexture(std::string& path, std::string& fileName) {}
//{
//	ILuint size;
//	ILubyte* data;
//
//	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
//
//	size = ilSaveL(IL_DDS, NULL, 0);
//	if (size > 0)
//	{
//		data = new ILubyte[size];
//		if (ilSaveL(IL_DDS, data, 0))
//		{
//
//		}
//	}
//
//	std::string result = path;
//	result.append("edgytexture_");
//	result.append(fileName);
//	result.append(".dds");
//
//}

bool ModuleFileSystem::LoadFromFile()
{
	std::string str = "Library\\Meshes\\";


	const std::experimental::filesystem::directory_iterator end{};

	for (std::experimental::filesystem::directory_iterator iter{ str.data() }; iter != end; ++iter)
	{
		if (std::experimental::filesystem::is_regular_file(*iter))
			CopyDataFromFile(iter->path().string());
	}

	return true;
}

void ModuleFileSystem::CopyDataFromFile(std::string& path)
{
	mesh* _mesh = new mesh();

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
	uint bytes = sizeof(ranges);
	
	memcpy(ranges, bookmark, bytes);

	path = path.erase(0, sizeof("Library\\Meshes\\")-1);
	_mesh->name = (char*)path.erase(path.find_last_of("."),sizeof(".edgy")).data();

	_mesh->num_vertex = ranges[0];
	_mesh->num_index = ranges[1];
	_mesh->num_faces = _mesh->num_index / 3;

	bookmark += bytes;
	bytes = sizeof(float)*_mesh->num_vertex * 3;

	_mesh->vertex = new float[_mesh->num_vertex * 3];
	memcpy(_mesh->vertex, bookmark, bytes);

	bookmark += bytes;
	bytes = sizeof(uint)*_mesh->num_index;

	_mesh->index = new uint[_mesh->num_index];
	memcpy(_mesh->index, bookmark, bytes);

	bookmark += bytes;
	bytes = sizeof(float)*_mesh->num_vertex * 2;

	_mesh->texCoords = new float[_mesh->num_vertex * 2];
	memcpy(_mesh->texCoords, bookmark, bytes);

	bookmark += bytes;
	bytes = sizeof(float)*_mesh->num_vertex * 3;

	_mesh->normals = new float[_mesh->num_vertex * 3];
	memcpy(_mesh->normals, bookmark, bytes);

	fclose(file);

	_mesh->scale.x = 100;
	_mesh->scale.y = 100;
	_mesh->scale.z = 100;

	_mesh->hasTextCoords = true;

	glGenBuffers(1, (GLuint*)&(_mesh->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * _mesh->num_index, &_mesh->index[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	App->renderer3D->mesh_list.push_back(_mesh);
}

void ModuleFileSystem::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleFileSystem::Load(rapidjson::Document& doc)
{
}