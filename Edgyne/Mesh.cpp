#include "Mesh.h"
#include "Application.h"
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
	SetBoundingVolume();
}


Mesh::~Mesh()
{
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
	vec translator = game_object->transform->position;
	vec global_scale = game_object->global_transform_matrix.Transposed().ExtractScale();
	translator.x *= global_scale.x;
	translator.y *=global_scale.y;
	translator.z *=global_scale.z;

	Quat global_rotation = game_object->global_transform_matrix.Transposed().RotatePart().ToQuat();

	std::vector<float3> rotated_mesh;

	vec tmp;
	for (int i = 0; i < num_vertex*3; i+=3)
	{

		tmp.x = vertex[i];
		tmp.y = vertex[i + 1];
		tmp.z = vertex[i + 2];
		rotated_mesh.push_back(tmp);
		rotated_mesh[rotated_mesh.size()-1] = global_rotation.Transform(rotated_mesh[rotated_mesh.size()-1]);
	}

	game_object->aligned_bounding_box.SetNegativeInfinity();
	game_object->aligned_bounding_box.Enclose((rotated_mesh.data()), num_vertex);
	game_object->aligned_bounding_box.maxPoint = translator + game_object->aligned_bounding_box.HalfDiagonal();
	game_object->aligned_bounding_box.minPoint = translator - game_object->aligned_bounding_box.HalfDiagonal();

	//for (int i = 0; i < num_vertex*3; i+3)
	//{
	//	game_object->bounding_box.Enclose((float3)vertex[i]);
	//}
	game_object->bounding_sphere.SetNegativeInfinity();
	
	game_object->bounding_sphere.Enclose((rotated_mesh.data()), num_vertex);
	
	game_object->bounding_sphere.Translate(translator);
}

void Mesh::TransformChanged()
{
	SetBoundingVolume();
}


