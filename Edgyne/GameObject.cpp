#include "GameObject.h"



GameObject::GameObject()
{
	aligned_bounding_box.SetNegativeInfinity();
}


GameObject::~GameObject()
{
}

bool GameObject::InitComponents()
{
	return false;
}

bool GameObject::UpdateComponents()
{
	return false;
}

bool GameObject::CleanUpComopnents()
{
	return false;
}
