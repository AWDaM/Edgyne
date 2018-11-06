#include "Mesh.h"
#include "Application.h"
#include "Material.h"

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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
	glEnableClientState(GL_VERTEX_ARRAY);

	/*if (material)
	{
		material->MaterialBind();
	}*/
	//--------
	if (id_texture)
	{
		glBindTexture(GL_TEXTURE_2D, id_texture);
	}

	/*else
		glColor3f(color.x, color.y, color.z);*/

	//---------
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if (has_texture_coordinates)
		glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);

	//Draw The Mesh
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	//Disable All The Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (id_texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return true;
}

void Mesh::SetBoundingVolume()
{
	game_object->aligned_bounding_box.Enclose((float3*)vertex, num_vertex);
	//for (int i = 0; i < num_vertex*3; i+3)
	//{
	//	game_object->bounding_box.Enclose((float3)vertex[i]);
	//}
	
	game_object->bounding_sphere.Enclose((float3*)vertex, num_vertex);
}

void Mesh::TransformChanged()
{
	
}
