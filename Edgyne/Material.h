#pragma once
#include "Component.h"
class Material :
	public Component
{
public:
	Material();
	Material(GameObject* game_object);
	~Material();
};

