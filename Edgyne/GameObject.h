#pragma once
#include "MathGeoLib/MathGeoLib.h"

#include <vector>

enum Tag {
	NO_TAG
};

enum Layer {
	NO_LAYER
};

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool InitComponents();
	bool UpdateComponents();
	bool CleanUpComopnents();

private:
public:
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

