#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleImporter.h"
#include "ModuleCamera3D.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include "Material.h"
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

#include "rapidjson/filereadstream.h"

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
	const aiScene* scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("-------Loading new mesh--------");
		aiNode* rootNode = scene->mRootNode;
		GameObject* rootGameObject = App->level->NewGameObject(rootNode->mName.C_Str());
		LoadAllNodesMeshes(rootNode, scene, file,rootGameObject);

		rootGameObject->RecursiveTransformChanged(App->level->root->global_transform_matrix);

		/*std::list<GameObject*>::iterator item = rootGameObject->childs.begin();
		while (item != rootGameObject->childs.end())
		{
			(*item)->SetLocalBoundingVolume();
			item++;
		}*/

		LOG("Centering Camera around the model");

		vec half_diagonal  = App->renderer3D->globalBoundingBox.CenterPoint();
		vec center_point = App->renderer3D->globalBoundingBox.CenterPoint();
		half_diagonal += App->renderer3D->globalBoundingBox.HalfDiagonal();
		 
		//App->camera->CameraAdaptation({ half_diagonal.x,half_diagonal.y,half_diagonal.z }, { center_point.x,center_point.y,center_point.z });
		aiReleaseImport(scene);
	}

	else
		LOG("Error loading object from path: %s", file);

	return true;
}

bool ModuleLoader::ImportTexture(const char* path)
{
	bool ret = true;


	App->importer->SaveTexture((std::string)path);

	float2 imgSize;
	ILuint img = ilGenImage();
	ilBindImage(img);

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

	ilDeleteImage(img);

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

void ModuleLoader::LoadInfo(GameObject* game_object, aiMesh * currentMesh, aiNode* node)
{

	aiQuaternion rotation;
	aiVector3D position, scaling, rotationEuler;

	node->mTransformation.Decompose(scaling, rotation, position);

	rotationEuler = rotation.GetEuler();

	
	game_object->transform->rotation.FromEulerXYZ(DEGTORAD*rotationEuler.x, DEGTORAD*rotationEuler.y, DEGTORAD*rotationEuler.z);
	game_object->transform->position.x = position.x;
	game_object->transform->position.y = position.y;
	game_object->transform->position.z = position.z;
	game_object->transform->scale.x = scaling.x;
	game_object->transform->scale.y = scaling.y;
	game_object->transform->scale.z = scaling.z;

	game_object->transform_changed = true;
	//new_mesh->num_faces = currentMesh->mNumFaces;

}

void ModuleLoader::LoadVerices(Mesh* new_mesh, aiMesh* currentMesh)
{
	new_mesh->num_vertex = currentMesh->mNumVertices;
	new_mesh->vertex = new float[new_mesh->num_vertex * 3];
	memcpy(new_mesh->vertex, currentMesh->mVertices, sizeof(float) * new_mesh->num_vertex * 3);

	LOG("New mesh with %d vertices", new_mesh->num_vertex);
}

void ModuleLoader::LoadColor(Material* new_material, aiMaterial* mat)
{
	aiColor3D color(1.f, 1.f, 1.f);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	new_material->color.x = color.r;
	new_material->color.y = color.g;
	new_material->color.z = color.b;
}

bool ModuleLoader::LoadTextures(Mesh* new_mesh, Material* _material, aiMesh* currentMesh, const aiScene* scene, const std::string& file)
{
	bool ret = true;
	if (currentMesh->HasTextureCoords(0))
	{
		new_mesh->has_texture_coordinates = true;
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
			std::string texFullPath;

			LOG("Image being loaded %s", texPath.C_Str());
			ILuint imgName;
			float2 imgSize;
			ilGenImages(1, &imgName);
			ilBindImage(imgName);
			if (CheckTexturePaths(file, texPath.C_Str(), texFullPath))
			{
				_material->fileName = SaveMaterial(texFullPath);
				ILinfo imgData;
				iluGetImageInfo(&imgData);
				if (imgData.Origin == IL_ORIGIN_UPPER_LEFT)
					iluFlipImage();

				_material->img_size.x = imgData.Width;
				_material->img_size.y = imgData.Height;

				if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
				{
					LOG("DevIL Error: %s", iluErrorString(ilGetError()));
					_material->id_texture = 0;
					ret = false;
				}
				else
				{
					glGenTextures(1, &_material->id_texture);
					glBindTexture(GL_TEXTURE_2D, _material->id_texture);

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
				_material->id_texture = 0;
				ret = false;
			}
			ilDeleteImage(imgName);
		}
		else
		{
			LOG("Error loading texture file");
			_material->id_texture = 0;
			ret = false;
		}
	}
	else
	{
		LOG("Item doesn't have texture coordinates");
		new_mesh->has_texture_coordinates = false;
	}
	return ret;
}

void ModuleLoader::LoadNormals(Mesh* new_mesh, aiMesh* currentMesh)
{
	new_mesh->normals = new float[new_mesh->num_vertex * 3];
	memcpy(new_mesh->normals, currentMesh->mNormals, sizeof(float)*new_mesh->num_vertex * 3);
	LOG("Normals loaded correctly");
}

void ModuleLoader::LoadIndices(Mesh* new_mesh, aiMesh* currentMesh)
{
	new_mesh->num_index = currentMesh->mNumFaces * 3; // assume each face is a triangle
	new_mesh->index = new uint[new_mesh->num_index]; 

	for (uint j = 0; j < currentMesh->mNumFaces; ++j)
	{
		if (currentMesh->mFaces[j].mNumIndices != 3)
		{
			LOG("---WARNING--- Geometry face with != 3 indices, Won't be drawn on screen");
			new_mesh->has_triangle_faces = false;
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

void ModuleLoader::LoadBoundingBox(Mesh * new_mesh, aiMesh * currentMesh)
{
	new_mesh->SetBoundingVolume();
	//AABB bounding_box;
	//bounding_box.SetNegativeInfinity();
	//bounding_box.Enclose((float3*)new_mesh->vertex, currentMesh->mNumVertices);
	//
	//new_mesh->bounding_box = bounding_box;
}

void ModuleLoader::LoadMeshesFromFile(Mesh* _mesh)
{

	//Index IDs



}

void ModuleLoader::LoadAllNodesMeshes(aiNode* node, const aiScene* scene, const std::string& file, GameObject* parent)
{
	GameObject* local_parent;
	if (node->mNumMeshes > 0)
	{
		local_parent = parent->AddGameObject("Node GameObject");
	}
	else
		local_parent = parent;
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		GameObject* game_object;
		if (node->mName.C_Str() != NULL)
		{
			game_object = local_parent->AddGameObject(node->mName.C_Str());
		}
		else
			game_object = local_parent->AddGameObject("Game Object");
		
		
		aiMesh* currentMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* mesh = (Mesh*)game_object->AddComponent(MESH);
		Material* material = (Material*)game_object->AddComponent(MATERIAL);
		mesh->material = material;


		LOG("Loading Info for the %i mesh", i + 1);
			LoadInfo(game_object, currentMesh, node);

		LOG("Loading Vertices from the %i mesh", i + 1);
			LoadVerices(mesh, currentMesh);
		
		LOG("Loading Color from the %i mesh", i + 1);
			LoadColor(material, scene->mMaterials[currentMesh->mMaterialIndex]);

		LOG("Loading Normals from the %i mesh", i + 1);
		if (currentMesh->HasNormals())
			LoadNormals(mesh, currentMesh);

		LOG("Loading Textures from the %i mesh", i + 1);
		if (currentMesh->HasTextureCoords(0))
			LoadTextures(mesh,material, currentMesh, scene, file);

		LOG("Loading Indices from the %i mesh", i + 1);
		if (currentMesh->HasFaces())
			LoadIndices(mesh, currentMesh);

		LOG("Generating BoundingBox for the %i mesh", i + 1);
			LoadBoundingBox(mesh, currentMesh);

		SaveMesh(mesh);

	}
	
	if (node->mNumChildren > 0)
	{
		for (int i = 0; i < node->mNumChildren; i++)
			LoadAllNodesMeshes(node->mChildren[i], scene, file,local_parent);
	}
}

bool ModuleLoader::CheckTexturePaths(std::string path, std::string texPath, std::string& texActualPath)
{
	bool ret = false;
	path = path.substr(0, path.find_last_of("\\")+1);
	if (texPath.find("\\") < texPath.size())
	{
		texPath.erase(0, texPath.find_first_of("\\")+1);
		std::string tmp_path = path;
		tmp_path.append(texPath.c_str());
		if (ilLoadImage(tmp_path.data()))
		{
			LOG("Texture found at the expected path");
			texActualPath = tmp_path;
			ret = true;
		}
		else
		{
			texPath.erase(0, texPath.find_last_of("\\")+1);
			path.append(texPath.c_str());
			if (ilLoadImage(path.data()))
			{
				LOG("Texture found at the same path as the object");
				texActualPath = path;
				ret = true;
			}
		}
	}
	else
	{
		path.append(texPath.c_str());
		if (ilLoadImage(path.data()))
		{
			LOG("Texture found at the expected path");
			texActualPath = path;
			ret = true;
		}
	}
	if(!ret)
	{
		path = "Assets\\";
		path.append(texPath.c_str());
		if (ilLoadImage(path.data()))
		{
			LOG("Texture found at the assets folder");
			texActualPath = path;
			ret = true;
		}
		else
		{
			path.clear();
			path.append(texPath.c_str());
			if (ilLoadImage(path.data()))
			{
				LOG("Texture found at the source folder");
				texActualPath = path;
				ret = true;
			}
		}
	}
	return ret;
}

void ModuleLoader::SaveScene(std::string name)
{
	rapidjson::Document document;
	document.SetObject();
	FILE* fp = fopen(name.append(".json").c_str(), "wb");
	char writeBuffer[1000000];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value scene(rapidjson::kObjectType);

	for (std::list<GameObject*>::iterator iterator = App->level->game_objects.begin(); iterator != App->level->game_objects.end(); iterator++)
	{

		rapidjson::Value obj(rapidjson::kObjectType);
		obj.AddMember("UID", (*iterator)->UID, allocator);
		obj.AddMember("Parent UID", (*iterator)->parentUID, allocator);
		obj.AddMember("Active", (*iterator)->active, allocator);
		obj.AddMember("Static", (*iterator)->Static, allocator);

		/*rapidjson::Value children(rapidjson::kObjectType);
		for (std::vector<uint>::iterator item = (*iterator)->childrenUID.begin(); item != (*iterator)->childrenUID.end(); item++)
			children.AddMember("UID", (*item), allocator);*/

		obj.AddMember("Object Name", (rapidjson::Value::StringRefType)(*iterator)->name.c_str(), allocator);

		if((*iterator)->childrenUID.size() > 0)
			obj.AddMember("Has Children", true, allocator);
		else
			obj.AddMember("Has Children", false, allocator);

		rapidjson::Value component(rapidjson::kObjectType);
		(*iterator)->SaveToScene(component, allocator);
		obj.AddMember("Components", component, allocator);

		scene.AddMember("Game Object", obj, allocator);
	}
	document.AddMember("Scene", scene, allocator);
	//for(rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {   //iterate through object   
	//	const  rapidjson::Value& objName = document[itr->name.GetString()];
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);

	fclose(fp);
}

void ModuleLoader::LoadScene(std::string name)
{
	rapidjson::Document document;

	FILE* file = fopen(name.append(".json").c_str(), "rb");
	if (file)
	{
		char readBuffer[65536];

		rapidjson::FileReadStream inputStream(file, readBuffer, sizeof(readBuffer));

		document.ParseStream(inputStream);

		AddGameObjectsFromFile(App->level->root, document);
	}
}

void ModuleLoader::AddGameObjectsFromFile(GameObject* parent, rapidjson::Document& document)
{
	const rapidjson::Value& scene = document["Scene"];

	for (rapidjson::Value::ConstMemberIterator itr = scene.MemberBegin(); itr != scene.MemberEnd(); itr++)
	{
		if (itr->value["Parent UID"].GetUint() != 0 && itr->value["Parent UID"].GetUint() == parent->UID)
		{
			GameObject* go = parent->AddGameObject(itr->value["Object Name"].GetString());
			go->active = itr->value["Active"].GetBool();
			go->Static = itr->value["Static"].GetBool();
			go->parentUID = itr->value["Parent UID"].GetUint();
			go->UID = itr->value["UID"].GetUint();
			parent->childrenUID.push_back(go->UID);

			if (itr->value["Has Children"].GetBool())
			{
				AddGameObjectsFromFile(go, document);
			}
			const rapidjson::Value& components = itr->value["Components"];

			for (rapidjson::Value::ConstMemberIterator comp = components.MemberBegin(); comp != components.MemberEnd(); comp++)
			{
				switch (comp->value["Type"].GetInt())
				{
				case ComponentType::TRANSFORM:
				{
					Transform* t = (Transform*)go->AddComponent(ComponentType::TRANSFORM); 
					t->LoadComponent(comp);
					break;
				}
				case ComponentType::CAMERA:
				{
					Camera* c = (Camera*)go->AddComponent(ComponentType::CAMERA);
					c->LoadComponent(comp);
					break;
				}
				case ComponentType::MATERIAL:
				{
					Material* ma = (Material*)go->AddComponent(ComponentType::MATERIAL);
					ma->LoadComponent(comp);
					break;
				}
				case ComponentType::MESH:
				{
					Mesh* me = (Mesh*)go->AddComponent(ComponentType::MESH);
					me->LoadComponent(comp);
					break;
				}
				default:
					break;
				}
			}

		}
	}
}

void ModuleLoader::SaveMesh(Mesh* mesh)
{
	uint ranges[2] = { mesh->num_vertex, mesh->num_index };
	bool optatives[3] = { mesh->has_texture_coordinates, mesh->has_triangle_faces, mesh->has_normals };
	uint fileSize;

	fileSize = sizeof(ranges) + sizeof(optatives) + sizeof(float)*mesh->num_vertex * 3 + sizeof(uint)*mesh->num_index;

	if (mesh->has_texture_coordinates)
		fileSize += sizeof(float)*mesh->num_vertex * 2;
	if(mesh->has_normals)
		fileSize += sizeof(float)*mesh->num_vertex * 3;

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

	if (mesh->has_texture_coordinates)
	{
		// Saving the data of the texture coordinates
		bytes = sizeof(float)*mesh->num_vertex * 2;
		memcpy(bookmark, mesh->texCoords, bytes);

		bookmark += bytes;
	}
	if (mesh->has_normals)
	{
		// Saving the data of the normals
		bytes = sizeof(float)*mesh->num_vertex * 3;
		memcpy(bookmark, mesh->normals, bytes);
	}

	std::string str = App->importer->meshLibraryPath;
	str.append(mesh->game_object->name);
	str.append(App->importer->meshExtension);

	App->importer->WriteDataOnFile(data, fileSize, str.c_str());
	mesh->fileName = str;
	mesh->fileName = mesh->fileName.erase(0, mesh->fileName.find_last_of("\\") + 1);
	mesh->fileName = mesh->fileName.substr(0, mesh->fileName.find("."));
}

std::string ModuleLoader::SaveMaterial(const std::string& path)
{
	std::string ddsTexPath = "No Path";
	std::string fileName = path;
	fileName = fileName.erase(0, fileName.find_last_of("\\") + 1);
	fileName = fileName.substr(0, fileName.find("."));

	uint size;
	const char* buffer = App->importer->LoadDataFromFile(path.c_str(), size);


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
				std::string currMaterialPath = App->importer->materialLibraryPath;
				currMaterialPath.append(fileName);
				currMaterialPath.append(App->importer->materialExtension);


				FILE* file = fopen(currMaterialPath.c_str(), "wb");
				fwrite(data, sizeof(char), ilSize, file);
				fclose(file);
				App->importer->WriteDataOnFile(data, ilSize, currMaterialPath.c_str());
				ddsTexPath = currMaterialPath;
				ddsTexPath = ddsTexPath.erase(0, ddsTexPath.find_last_of("\\") + 1);
				ddsTexPath = ddsTexPath.substr(0, ddsTexPath.find("."));
			}
		}
	}

	return ddsTexPath;
}

void ModuleLoader::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

void ModuleLoader::Load(rapidjson::Document& doc)
{
}
