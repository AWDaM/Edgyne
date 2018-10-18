#pragma once
#include "GameObject.h"

enum ComponentType
{
	NO_TYPE,
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
public:
	Component();
	Component(GameObject* game_object, ComponentType type);
	~Component();

	virtual void OnEditor() {};

private:
public:
	bool active = false;
	GameObject* game_object = nullptr;
	ComponentType component_type;
private:

};

