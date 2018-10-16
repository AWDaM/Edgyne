#pragma once
class GameObject;

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
	~Component();

	virtual void OnEditor() {};
private:
public:
	bool active;
	GameObject* game_object = nullptr;
	ComponentType component_type;
private:

};

