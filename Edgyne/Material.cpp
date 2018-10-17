#include "Material.h"



Material::Material()
{
}

Material::Material(GameObject * game_object) : Component(game_object,MATERIAL)
{
}


Material::~Material()
{
}
