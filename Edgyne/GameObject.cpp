#include "GameObject.h"
#include "Globals.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "ImGui\imgui.h"


GameObject::GameObject(std::string _name)
{
	aligned_bounding_box.SetNegativeInfinity();
	name = _name;
}


GameObject::~GameObject()
{
}

bool GameObject::InitComponents()
{
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->ComponentStart();
		item++;
	}
	return true;
}

bool GameObject::UpdateComponents()
{
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->ComponentUpdate();
		item++;
	}
	return false;
}

bool GameObject::CleanUpComopnents()
{
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->ComponentCleanUp();
		item++;
	}
	return false;
}

void GameObject::OnHierarchy()
{
	std::vector<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		if (ImGui::TreeNode((*item)->name.c_str()))
		{
			(*item)->OnHierarchy();
		}

		item++;
	}
}

Component * GameObject::AddComponent(ComponentType type)
{
	Component* ret;
	switch (type)
	{
	case NO_TYPE:
		LOG("no component type was specified");
		break;
	case TRANSFORM:
		ret = new Transform(this);
		break;
	case MESH:
		ret = new Mesh(this);
		break;
	case MATERIAL:
		ret = new Material(this);
		break;
	case CAMERA:
		ret = new Camera(this);
		break;
	default:
		break;
	}
	return ret;
}
