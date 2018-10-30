#include "Application.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "Globals.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "ImGui\imgui.h"


GameObject::GameObject(GameObject* parent, std::string _name) : active(true), parent(parent)
{
	aligned_bounding_box.SetNegativeInfinity();
	name = _name;
}


GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	UpdateComponents();
	return true;
}

bool GameObject::Draw()
{
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->ComponentDraw();
		item++;
	}
	return true;
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
		if (!(*item)->active)
		{
			ImGui::PushStyleColor(1, { 128,128,128,128 });
		}
		if (ImGui::TreeNode((*item)->name.c_str()))
		{
			(*item)->OnHierarchy();
			App->level->selected_game_object = (*item);
			ImGui::TreePop();
		}
		if (!(*item)->active)
		{
			ImGui::PopStyleColor();
		}
		item++;
	}
}

void GameObject::OnInspector()
{
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
	ret->ComponentStart();
	components.push_back(ret);
	return ret;
}

GameObject * GameObject::AddGameObject(std::string name, bool with_transform)
{
	GameObject* ret = new GameObject(this,name);
	if (with_transform)
	{
		ret->AddComponent(TRANSFORM);
	}
	App->level->game_objects.push_back(ret);
	childs.push_back(ret);
	return ret;
}
