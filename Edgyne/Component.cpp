#include "Component.h"



Component::Component()
{
}

Component::Component(GameObject * game_object, ComponentType type) : game_object(game_object),component_type(type)
{
}


Component::~Component()
{
}
