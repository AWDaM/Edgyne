#include "Mesh.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"
#include "Resource.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "Material.h"
#include "Transform.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <vector>
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")


Mesh::Mesh()
{
}

Mesh::Mesh(GameObject * game_object) : Component(game_object,MESH)
{
}


Mesh::~Mesh()
{
}

rapidjson::Value Mesh::SaveToScene(rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value myData(rapidjson::kObjectType);

	myData.AddMember("UID", UID, allocator);
	myData.AddMember("Type", component_type, allocator);
	myData.AddMember("Resource UID", (rapidjson::Value::StringRefType)resource_mesh.c_str(), allocator);
	myData.AddMember("Material Name", (rapidjson::Value::StringRefType)material_name.c_str(), allocator);

	rapidjson::Value mat(rapidjson::kObjectType);

	if (material)
	{
		mat = material->SaveToScene(allocator);
		myData.AddMember("Material", mat, allocator);
	}


	return myData;
}

void Mesh::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	//UID = comp->value["UID"].GetUint();
	//resource_mesh->resource_uid = UID;
	//resource_mesh->has_texture_coordinates = comp->value["Texture Coordinates"].GetBool();
	//resource_mesh->has_triangle_faces = comp->value["Triangle Faces"].GetBool();
	//resource_mesh->has_normals = comp->value["Normals"].GetBool();
	//resource_mesh->fileName = comp->value["Mesh File Name"].GetString();

	//std::string tmp = "Library\\Meshes\\";

	//tmp += resource_mesh->fileName;
	//tmp += ".edgymesh";

	//App->importer->CopyDataFromFile(tmp, this);

	//material = new Material();

	//material->LoadAsMeshComponent(comp);
	UID = comp->value["UID"].GetUint();
	resource_mesh = comp->value["Resource UID"].GetString();

	if (!App->resource_manager->GetResourceFromUID(resource_mesh))
	{
		App->resource_manager->CreateNewResource(Resource::RESOURCE_MESH, resource_mesh);
	}

	material_name = comp->value["Material Name"].GetString();

	if (!App->resource_manager->GetResourceFromUID(material_name))
	{
		App->resource_manager->CreateNewResource(Resource::RES_MATERIAL, material_name);
	}

}

void Mesh::OnEditor()
{
	ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->GetResourceFromUID(this->resource_mesh);
	if (ImGui::TreeNode("Mesh"))
	{
		ImGui::Text("Num Faces: %i", resource_mesh->num_index/3);
		ImGui::Text("Num Vertices: %i", resource_mesh->num_vertex);
		ImGui::Text("Num Indexes: %i", resource_mesh->num_index);
		ImGui::TreePop();
	}
}

bool Mesh::ComponentUpdate()
{
	return true;
}

bool Mesh::ComponentDraw()
{
	ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->GetResourceFromUID(this->resource_mesh);

	if (resource_mesh->has_triangle_faces && resource_mesh->id_index > 0)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		ResourceMaterial* material = nullptr;
		if (this->material)
		{
			material = (ResourceMaterial*)App->resource_manager->GetResourceFromUID(this->material->resource_uid);
			if (material)
			{
				if (material->id_texture)
				{
					glBindTexture(GL_TEXTURE_2D, material->id_texture);
				}
				//--------

				else
					glColor3f(material->color.x, material->color.y, material->color.z);
			}
		}
			//---------
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource_mesh->id_index);
		glVertexPointer(3, GL_FLOAT, 0, &resource_mesh->vertex[0]);

		if (resource_mesh->has_texture_coordinates)
			glTexCoordPointer(2, GL_FLOAT, 0, &resource_mesh->texCoords[0]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//Draw The Mesh
		glDrawElements(GL_TRIANGLES, resource_mesh->num_index, GL_UNSIGNED_INT, NULL);

		//Disable All The Data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		if (this->material)
		{
			if (material)
			{
				if (material->id_texture)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				else
					glColor3f(1, 1, 1);
			}
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}
	return true;
}

void Mesh::SetBoundingVolume()
{
	ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->GetResourceFromUID(this->resource_mesh);

	game_object->aligned_bounding_box.Enclose((float3*)resource_mesh->vertex, resource_mesh->num_vertex);
	//for (int i = 0; i < num_vertex * 3; i + 3)
	//{
	//	game_object->bounding_box.Enclose()
	//}
	game_object->bounding_sphere.Enclose((float3*)resource_mesh->vertex, resource_mesh->num_vertex);
}



