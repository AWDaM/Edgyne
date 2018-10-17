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
