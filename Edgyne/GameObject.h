#pragma once
#include "MathGeoLib/MathGeoLib.h"

#include <vector>

enum ComponentType
{
	NO_TYPE,
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA
};

enum Tag {
	NO_TAG,
	MAIN_CAMERA
};

enum Layer {
	NO_LAYER
};

class Component;

class GameObject
{
public:
	GameObject(std::string _name = "new GameObject");
	~GameObject();

	bool InitComponents();
	bool UpdateComponents();
	bool CleanUpComopnents();

	void OnHierarchy();

	void OnInspector();

	Component* AddComponent(ComponentType type);

private:
public:
	std::string name;
	bool Static = false;
	bool active = false;
	GameObject* parent = nullptr;

	AABB aligned_bounding_box;
	Sphere bounding_sphere;
	OBB bounding_box;

private:
	std::vector<GameObject*> childs;
	std::vector<Component*> components;
};

