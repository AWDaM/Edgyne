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

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")
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
	const aiScene* scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			mesh* new_mesh = new mesh();
			aiMesh* currentMesh = scene->mMeshes[i];

			new_mesh->num_vertex = currentMesh->mNumVertices;
			new_mesh->vertex = new float[new_mesh->num_vertex * 3];
			memcpy(new_mesh->vertex, currentMesh->mVertices, sizeof(float) * new_mesh->num_vertex * 3);
			LOG("New mesh with %d vertices", new_mesh->num_vertex);

			if (currentMesh->HasFaces())
			{
				new_mesh->num_index = currentMesh->mNumFaces * 3;
				new_mesh->index = new uint[new_mesh->num_index]; // assume each face is a triangle
				for (uint i = 0; i < currentMesh->mNumFaces; ++i)
				{
					if (currentMesh->mFaces[i].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&new_mesh->index[i * 3], currentMesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}
			glGenBuffers(1, (GLuint*)&(new_mesh->id_index));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_index, &new_mesh->index[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			App->renderer3D->mesh_list.push_back(new_mesh);
		}
		aiReleaseImport(scene);
	}

	else
		LOG("Error loading scene %s", file);

	return true;
}
