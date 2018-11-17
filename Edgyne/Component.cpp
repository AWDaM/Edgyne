#include "Component.h"
#include "pcg_variants.h"
#include "Application.h"

Component::Component()
{
}

Component::Component(GameObject* game_object, ComponentType type) : game_object(game_object),component_type(type),active(true)
{
	UID = pcg32_random_r(&App->rng);
}


Component::~Component()
{
	std::vector<Component*>::iterator it = game_object->components.begin();
	while (it != game_object->components.end())
	{
		if ((*it) == this)
		{
			game_object->components.erase(it);
			break;
		}
		it++;
	}
	game_object = nullptr;
}
