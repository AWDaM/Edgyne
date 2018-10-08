#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleRenderer3D.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")
//#include "Assimp\include\cfileio.h"

#pragma comment(lib,"Assimp/libx86/assimp.lib")


ModuleLoader::ModuleLoader(Application * app, bool start_enabled) : Module(start_enabled)
{
	name = "loader";
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Init(rapidjson::Document& document)
{
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	ILenum error;
	error = iluLoadImage("Lenna_(test_image).png");
	LOG("%d: %s/n", error, iluErrorString(error));
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

update_status ModuleLoader::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	return ret;
}

bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

bool ModuleLoader::Import(const std::string & file)
{
	const aiScene* scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;
		aiNode* currentNode = rootNode;
	
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			mesh* new_mesh = new mesh();
			aiMesh* currentMesh = scene->mMeshes[i];

			LOG("Loading Vertices from the %i mesh", i + 1);
			LoadVerices(new_mesh, currentMesh);

			LOG("Loading Normals from the %i mesh", i + 1);
			LoadNormals(new_mesh, currentMesh);

			LOG("Loading Textures from the %i mesh", i + 1);
			LoadTextures(new_mesh, currentMesh, scene);
			 
			LOG("Loading Indices from the %i mesh", i + 1);
			LoadIndices(new_mesh, currentMesh);

			
			App->renderer3D->mesh_list.push_back(new_mesh);
		}
		aiReleaseImport(scene);
	}

	else
		LOG("Error loading scene %s", file);

	return true;
}

bool ModuleLoader::ImportTexture(const char* path)
{
	bool ret = true;
	ILuint imgName;
	ilGenImages(1, &imgName);
	ilBindImage(imgName);
	uint dropped_texture = 0;
	if (ilLoadImage(path))
	{
		ILinfo imgData;
		iluGetImageInfo(&imgData);
		if (imgData.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			LOG("DevIL Error: %s", iluErrorString(ilGetError()));
			ret = false;
		}
		else
		{
			
			glGenTextures(1, &dropped_texture);
			glBindTexture(GL_TEXTURE_2D, dropped_texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.Width, imgData.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else
	{
		ret = false;
	}
	ilDeleteImage(imgName);

	std::list<mesh*>::iterator item = App->renderer3D->mesh_list.begin();

	while (item != App->renderer3D->mesh_list.end())
	{
		(*item)->id_texture = dropped_texture;

		item++;
	}

	return ret;
}

void ModuleLoader::ReceivedFile(const char * path)
{
	std::string path_string = path;
	path_string.erase(0, path_string.size() - 3);
	if (path_string == MODEL)
	{
		Import(path);
	}
	else if (path_string == IMAGE)
	{
		ImportTexture(path);
	}
}

void ModuleLoader::LoadVerices(mesh* new_mesh, aiMesh* currentMesh)
{
	new_mesh->num_vertex = currentMesh->mNumVertices;
	new_mesh->vertex = new float[new_mesh->num_vertex * 3];
	memcpy(new_mesh->vertex, currentMesh->mVertices, sizeof(float) * new_mesh->num_vertex * 3);

	LOG("New mesh with %d vertices", new_mesh->num_vertex);
}

bool ModuleLoader::LoadTextures(mesh* new_mesh, aiMesh* currentMesh, const aiScene* scene)
{
	bool ret = true;
	if (currentMesh->HasTextureCoords(0))
	{
		new_mesh->texCoords = new float[new_mesh->num_vertex * 2];

		for (int k = 0; k < new_mesh->num_vertex * 2; k += 2)
		{
			new_mesh->texCoords[k] = currentMesh->mTextureCoords[0][k/2].x;
			new_mesh->texCoords[k+1] = currentMesh->mTextureCoords[0][k/2].y;
		}

		aiMaterial* material = scene->mMaterials[currentMesh->mMaterialIndex];
		aiString texPath;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		std::string texFullPath = "Library/";
		texFullPath.append(texPath.C_Str());
		LOG("Image being loaded %s", texPath.C_Str());
		ILuint imgName;
		ilGenImages(1, &imgName);
		ilBindImage(imgName);
		if (ilLoadImage(texFullPath.data()))
		{
			ILinfo imgData;
			iluGetImageInfo(&imgData);
			if (imgData.Origin == IL_ORIGIN_UPPER_LEFT)
				iluFlipImage();

			if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				LOG("DevIL Error: %s", iluErrorString(ilGetError()));
				ret = false;
			}
			else
			{
				glGenTextures(1, &new_mesh->id_texture);
				glBindTexture(GL_TEXTURE_2D, new_mesh->id_texture);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.Width, imgData.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else
		{
			ret = false;
		}
		ilDeleteImage(imgName);
	}
	return ret;
}

void ModuleLoader::LoadNormals(mesh* new_mesh, aiMesh* currentMesh)
{
	new_mesh->normals = new float[new_mesh->num_vertex * 3];
	memcpy(new_mesh->normals, currentMesh->mNormals, sizeof(float)*new_mesh->num_vertex * 3);
	LOG("Normals loaded correctly");
}

void ModuleLoader::LoadIndices(mesh* new_mesh, aiMesh* currentMesh)
{
	if (currentMesh->HasFaces())
	{
		new_mesh->num_index = currentMesh->mNumFaces * 3;
		new_mesh->index = new uint[new_mesh->num_index]; // assume each face is a triangle

		for (uint j = 0; j < currentMesh->mNumFaces; ++j)
		{
			if (currentMesh->mFaces[j].mNumIndices != 3)
				LOG("WARNING, geometry face with != 3 indices!");
			else
				memcpy(&new_mesh->index[j * 3], currentMesh->mFaces[j].mIndices, 3 * sizeof(uint));
		}
		LOG("New mesh with %d indices", new_mesh->num_index);
	}
	glGenBuffers(1, (GLuint*)&(new_mesh->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_index, &new_mesh->index[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



