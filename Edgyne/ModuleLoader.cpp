#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleRenderer3D.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")


#pragma comment(lib,"Assimpl/libx86/assimp.lib")


ModuleLoader::ModuleLoader(Application * app, bool start_enabled) : Module(start_enabled)
{
	name = "loader";
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Init(rapidjson::Document& document)
{

	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleLoader::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

bool ModuleLoader::Import(const std::string & file)
{
	const aiScene* scene = aiImportFile(file.c_str, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			LOG("Loading new mesh with: /n");
			mesh* new_mesh = new mesh();
			new_mesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			new_mesh->vertex = new float[new_mesh->num_vertex * 3];
			memcpy(new_mesh->vertex, scene->mMeshes[i], sizeof(float)*new_mesh->num_vertex * 3);
			LOG("%d vertices", new_mesh->num_vertex);

			if (scene->mMeshes[i]->HasFaces())
			{
				new_mesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				new_mesh->index = new uint[new_mesh->num_index];
				for (uint i = 0; i < scene->mMeshes[i]->mNumFaces; i++)
				{
					if (scene->mMeshes[i]->mFaces[i].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices !!!");
					else
						memcpy(&new_mesh->index[i * 3], scene->mMeshes[i]->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", file);

	return true;
}
