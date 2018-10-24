#pragma once
#include "GameObject.h"



class Component
{
public:
	Component();
	Component(GameObject* game_object, ComponentType type);
	~Component();

	virtual bool ComponentStart() { return true; };
	virtual bool ComponentUpdate() { return true; };
	virtual bool ComponentCleanUp() { return true; };
	virtual bool ComponentDraw() { return true; };

	virtual void OnEditor() {};

private:
public:
	bool active = false;
	GameObject* game_object = nullptr;
	ComponentType component_type;
private:

};

