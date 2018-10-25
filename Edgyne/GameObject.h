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

	bool Update();
	bool Draw();

	bool InitComponents();
	bool UpdateComponents();
	bool CleanUpComopnents();

	void OnHierarchy();

	void OnInspector();

	Component* AddComponent(ComponentType type);
	GameObject* AddGameObject(std::string name, bool with_transform);

private:
public:
	std::string name;
	bool Static = false;
	bool active = false;
	GameObject* parent = nullptr;

	Tag tag = NO_TAG;
	AABB aligned_bounding_box;
	Sphere bounding_sphere;
	OBB bounding_box;
	std::vector<Component*> components;

private:
	std::vector<GameObject*> childs;

};

