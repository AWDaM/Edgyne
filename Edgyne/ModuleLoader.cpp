#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
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
#include "Assimp\include\cfileio.h"

#pragma comment(lib,"Assimp/libx86/assimp.lib")


void AssimpLoggerLoad(const char* message, char* user)
{
	LOG("%s", message);
}


ModuleLoader::ModuleLoader(Application * app, bool start_enabled) : Module(start_enabled)
{
	name = "Loader";
}

ModuleLoader::~ModuleLoader()
{
}

bool ModuleLoader::Init(rapidjson::Value& node)
{
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpLoggerLoad;
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
	MeshPath = file;
	TexturePath.clear();
	App->renderer3D->DeleteMesh();
	App->renderer3D->mesh_list.clear();
	const aiScene* scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("-------Loading new mesh--------");
		aiNode* rootNode = scene->mRootNode;
		LoadAllNodesMeshes(rootNode, scene, file);
		
		LOG("Centering Camera around the model");
		App->renderer3D->CalculateGlobalBoundingBox();
		vec half_diagonal  = App->renderer3D->globalBoundingBox.CenterPoint();
		vec center_point = App->renderer3D->globalBoundingBox.CenterPoint();
		half_diagonal += App->renderer3D->globalBoundingBox.HalfDiagonal();
		 
		App->camera->CameraAdaptation({ half_diagonal.x,half_diagonal.y,half_diagonal.z }, { center_point.x,center_point.y,center_point.z });
		aiReleaseImport(scene);
	}

	else
		LOG("Error loading object from path: %s", file);

	return true;
}

bool ModuleLoader::ImportTexture(const char* path)
{
	TexturePath = path;
	bool ret = true;
	ILuint imgName;
	float2 imgSize;
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
			imgSize.x = imgData.Width;
			imgSize.y = imgData.Height;
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
		(*item)->image_size = imgSize;
		item++;
	}

	return ret;
}

void ModuleLoader::ReceivedFile(const char * path)
{
	std::string path_string = path;
	path_string.erase(0,path_string.find_last_of(".")+1);
	if (MODEL(path_string))
	{
		Import(path);
	}
	else if (IMAGE(path_string))
	{
		ImportTexture(path);
	}
}

void ModuleLoader::LoadInfo(mesh * new_mesh, aiMesh * currentMesh, aiNode* node)
{
	if (currentMesh->mName.length != NULL)
	{
		memcpy(new_mesh->name, currentMesh->mName.C_Str(), currentMesh->mName.length);
	}
	else
		memcpy(new_mesh->name, "No_name", 8);


	aiQuaternion rotation;
	aiVector3D position, scaling, rotationEuler;

	node->mTransformation.Decompose(scaling, rotation, position);

	rotationEuler = rotation.GetEuler();

	new_mesh->rotation.x = math::RadToDeg(rotationEuler.x);
	new_mesh->rotation.y = math::RadToDeg(rotationEuler.y);
	new_mesh->rotation.z = math::RadToDeg(rotationEuler.z);
	new_mesh->position.x = position.x;
	new_mesh->position.y = position.y;
	new_mesh->position.z = position.z;
	new_mesh->scale.x = scaling.x;
	new_mesh->scale.y = scaling.y;
	new_mesh->scale.z = scaling.z;

	new_mesh->num_faces = currentMesh->mNumFaces;

}

void ModuleLoader::LoadVerices(mesh* new_mesh, aiMesh* currentMesh)
{
	new_mesh->num_vertex = currentMesh->mNumVertices;
	new_mesh->vertex = new float[new_mesh->num_vertex * 3];
	memcpy(new_mesh->vertex, currentMesh->mVertices, sizeof(float) * new_mesh->num_vertex * 3);

	LOG("New mesh with %d vertices", new_mesh->num_vertex);
}

void ModuleLoader::LoadColor(mesh* new_mesh, aiMaterial* mat)
{
	aiColor3D color(1.f, 1.f, 1.f);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	new_mesh->color.x = color.r;
	new_mesh->color.y = color.g;
	new_mesh->color.z = color.b;
}

bool ModuleLoader::LoadTextures(mesh* new_mesh, aiMesh* currentMesh, const aiScene* scene, const std::string& file)
{
	bool ret = true;
	if (currentMesh->HasTextureCoords(0))
	{
		new_mesh->hasTextCoords = true;
		new_mesh->texCoords = new float[new_mesh->num_vertex * 2];

		for (int k = 0; k < new_mesh->num_vertex * 2; k += 2)
		{
			new_mesh->texCoords[k] = currentMesh->mTextureCoords[0][k/2].x;
			new_mesh->texCoords[k+1] = currentMesh->mTextureCoords[0][k/2].y;
		}

		aiMaterial* material = scene->mMaterials[currentMesh->mMaterialIndex];
		aiString texPath;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		if (texPath.length > 0)
		{
			std::string texFullPath = file;

			LOG("Image being loaded %s", texPath.C_Str());
			ILuint imgName;
			float2 imgSize;
			ilGenImages(1, &imgName);
			ilBindImage(imgName);
			if (CheckTexturePaths(file, texPath))
			{
				ILinfo imgData;
				iluGetImageInfo(&imgData);
				if (imgData.Origin == IL_ORIGIN_UPPER_LEFT)
					iluFlipImage();

				new_mesh->image_size.x = imgData.Width;
				new_mesh->image_size.y = imgData.Height;

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
				LOG("Error loading texture file");
				ret = false;
			}
			ilDeleteImage(imgName);
		}
		else
		{
			LOG("Error loading texture file");
			ret = false;
		}
	}
	else
	{
		LOG("Item doesn't have texture coordinates");
		new_mesh->hasTextCoords = false;
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
	new_mesh->num_index = currentMesh->mNumFaces * 3; // assume each face is a triangle
	new_mesh->index = new uint[new_mesh->num_index]; 

	for (uint j = 0; j < currentMesh->mNumFaces; ++j)
	{
		if (currentMesh->mFaces[j].mNumIndices != 3)
		{
			LOG("---WARNING--- Geometry face with != 3 indices, Won't be drawn on screen");
			new_mesh->hasTriangleFaces = false;
			break;

		}
		else
			memcpy(&new_mesh->index[j * 3], currentMesh->mFaces[j].mIndices, 3 * sizeof(uint));
	}
	LOG("New mesh with %d indices", new_mesh->num_index);

	glGenBuffers(1, (GLuint*)&(new_mesh->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_index, &new_mesh->index[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleLoader::LoadBoundingBox(mesh * new_mesh, aiMesh * currentMesh)
{
	
	AABB bounding_box;
	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose((float3*)new_mesh->vertex, currentMesh->mNumVertices);
	
	new_mesh->bounding_box = bounding_box;
}

void ModuleLoader::LoadAllNodesMeshes(aiNode* node, const aiScene* scene, const std::string& file)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		mesh* new_mesh = new mesh();
		
		aiMesh* currentMesh = scene->mMeshes[node->mMeshes[i]];
		LOG("Loading Info for the %i mesh", i + 1);
		LoadInfo(new_mesh, currentMesh, node);

		LOG("Loading Vertices from the %i mesh", i + 1);
		LoadVerices(new_mesh, currentMesh);

		LOG("Loading Color from the %i mesh", i + 1);
		LoadColor(new_mesh, scene->mMaterials[currentMesh->mMaterialIndex]);

		LOG("Loading Normals from the %i mesh", i + 1);
		if (currentMesh->HasNormals())
			LoadNormals(new_mesh, currentMesh);

		LOG("Loading Textures from the %i mesh", i + 1);
		if (currentMesh->HasTextureCoords(0))
			LoadTextures(new_mesh, currentMesh, scene, file);


		LOG("Loading Indices from the %i mesh", i + 1);
		if (currentMesh->HasFaces())
			LoadIndices(new_mesh, currentMesh);

		LOG("Generating BoundingBox for the %i mesh", i + 1);
		LoadBoundingBox(new_mesh, currentMesh);

		App->renderer3D->mesh_list.push_back(new_mesh);
		App->fileSystem->SaveToFile(new_mesh);
	}
	
	if (node->mNumChildren > 0)
	{
		for (int i = 0; i < node->mNumChildren; i++)
			LoadAllNodesMeshes(node->mChildren[i], scene, file);
	}
}

bool ModuleLoader::CheckTexturePaths(std::string file, aiString texPath)
{
	bool ret = false;
	file = file.substr(0,file.find_last_of("\\")+1);
	file.append(texPath.C_Str());
	if (ilLoadImage(file.data()))
	{
		TexturePath = file;
		LOG("Texture found at the same file as the object");
		ret = true;
	}
	else
	{
		file = "Assets/";
		file.append(texPath.C_Str());
		if (ilLoadImage(file.data()))
		{
			TexturePath = file;
			LOG("Texture found at the library folder");
			ret = true;
		}
		else
		{
			file.clear();
			file.append(texPath.C_Str());
			if (ilLoadImage(file.data()))
			{
				TexturePath = file;
				LOG("Texture found at the source folder");
				ret = true;
			}
		}
	}
	return ret;
}

void ModuleLoader::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember("Mesh Path", (rapidjson::Value::StringRefType)MeshPath.data(), allocator);
	if(TexturePath.size())
		obj.AddMember("Texture Path", (rapidjson::Value::StringRefType)TexturePath.data(), allocator);
	
	doc.AddMember((rapidjson::Value::StringRefType)name.data(), obj, allocator);

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
}

void ModuleLoader::Load(rapidjson::Document& doc)
{
	rapidjson::Value& node = doc[name.data()];

	MeshPath = node["Mesh Path"].GetString();
	Import(MeshPath);
	if (node.HasMember("Texture Path"))
	{
		TexturePath = node["Texture Path"].GetString();
		ImportTexture(TexturePath.data());
	}

}
