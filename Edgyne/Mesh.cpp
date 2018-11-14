#include "Mesh.h"
#include "Application.h"
#include "ModuleImporter.h"
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
	myData.AddMember("Mesh File Name", (rapidjson::Value::StringRefType)fileName.c_str(), allocator);
	myData.AddMember("Texture Coordinates", has_texture_coordinates, allocator);
	myData.AddMember("Normals", has_normals, allocator);
	myData.AddMember("Triangle Faces", has_triangle_faces, allocator);

	rapidjson::Value mat(rapidjson::kObjectType);

	mat = material->SaveToScene(allocator);

	myData.AddMember("Material", mat, allocator);

	return myData;
}

void Mesh::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["UID"].GetUint();
	has_texture_coordinates = comp->value["Texture Coordinates"].GetBool();
	has_triangle_faces = comp->value["Triangle Faces"].GetBool();
	has_normals = comp->value["Normals"].GetBool();
	fileName = comp->value["Mesh File Name"].GetString();

	App->importer->CopyDataFromFile(App->importer->FindFileInFolder(fileName));

	material = new Material();

	material->LoadAsMeshComponent(comp);

}

void Mesh::OnEditor()
{
	if (ImGui::TreeNode("Mesh"))
	{
		ImGui::Text("Num Faces: %i", num_index/3);
		ImGui::Text("Num Vertices: %i", num_vertex);
		ImGui::Text("Num Indexes: %i", num_index);
		ImGui::TreePop();
	}
}

bool Mesh::ComponentUpdate()
{
	return true;
}

bool Mesh::ComponentDraw()
{
	if (has_triangle_faces)
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		if (material->id_texture)
		{
			glBindTexture(GL_TEXTURE_2D, material->id_texture);
		}
		//--------

		else
			glColor3f(material->color.x, material->color.y, material->color.z);

			//---------
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);

		if (has_texture_coordinates)
			glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//Draw The Mesh
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

		//Disable All The Data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		if (material->id_texture)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		else
			glColor3f(1, 1, 1);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}
	return true;
}

void Mesh::SetBoundingVolume()
{
	game_object->aligned_bounding_box.Enclose((float3*)vertex, num_vertex);
	//for (int i = 0; i < num_vertex * 3; i + 3)
	//{
	//	game_object->bounding_box.Enclose()
	//}
	game_object->bounding_sphere.Enclose((float3*)vertex, num_vertex);
}

void Mesh::TransformChanged()
{

}


