#include "Mesh.h"
#include "Application.h"



Mesh::Mesh()
{
}

Mesh::Mesh(GameObject * game_object) : Component(game_object,MESH)
{
}


Mesh::~Mesh()
{
}

void Mesh::OnEditor()
{

}

void Mesh::SetBoundingVolume()
{
	game_object->aligned_bounding_box.Enclose((float3*)vertex, num_vertex);
	game_object->bounding_box.OptimalEnclosingOBB((float3*)vertex, num_vertex);
	game_object->bounding_sphere.Enclose((float3*)vertex, num_vertex);
}
