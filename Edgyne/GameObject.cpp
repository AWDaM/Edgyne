#include "Application.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "Globals.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "GL\glew.h"
#include "ImGui\imgui.h"


GameObject::GameObject(GameObject* parent, std::string _name) : active(true), parent(parent)
{
	aligned_bounding_box.SetNegativeInfinity();
	name = _name;
	global_transform_matrix.SetIdentity();
}


GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	UpdateComponents();
	if (transform_changed)
	{
		std::vector<Component*>::iterator item = components.begin();

		while (item != components.end())
		{
			(*item)->TransformChanged();
			item++;
		}
		transform_changed = false;
	}
	return true;
}

bool GameObject::Draw()
{
	glPushMatrix();
	glMultMatrixf(global_transform_matrix.Transposed().ptr());
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->ComponentDraw();
		item++;
	}
	glPopMatrix();
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
	return true;
}

void GameObject::OnHierarchy(int id)
{
	std::list<GameObject*>::iterator item = childs.begin();
	while (item != childs.end())
	{

		if (!(*item)->active)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,0.5f,0.5f });
		}


		id++;
		ImGui::PushID(id);

		if ((*item) == App->level->selected_game_object)
		{
			//ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f,1.0f,0.0f,1.0f });
		}
		if (ImGui::TreeNode((*item)->name.c_str()))
		{
			if ((*item) == App->level->selected_game_object)
			{
				//ImGui::PopStyleColor();
			}

			if (ImGui::IsItemClicked())
				App->level->selected_game_object = (*item);

			(*item)->OnHierarchy(id);
			ImGui::TreePop();
		}

		ImGui::PopID();
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

void GameObject::RecursiveSetChildsActive(bool _active)
{
	active = _active;
	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->RecursiveSetChildsActive(_active);
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
	ret->ComponentStart();
	components.push_back(ret);
	return ret;
}

GameObject * GameObject::AddGameObject(std::string name, bool with_transform)
{
	GameObject* ret = new GameObject(this,name);
	if (with_transform)
	{
		ret->transform = (Transform*)ret->AddComponent(TRANSFORM);
	}
	App->level->game_objects.push_back(ret);
	childs.push_back(ret);
	return ret;
}

void GameObject::CalcGlobalTransform(const float4x4 & parent)
{
	global_transform_matrix = parent * float4x4::FromTRS(transform->position, transform->rotation, transform->scale);

	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->CalcGlobalTransform(global_transform_matrix);
		item++;
	}
}

void GameObject::RecursiveSetToDelete()
{
	to_remove = true;

	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->RecursiveSetToDelete();
		item++;
	}
}

void GameObject::RemoveSelfFromParent()
{
	std::list<GameObject*>::iterator self_item = parent->childs.begin();

	while (self_item != parent->childs.end())
	{
		if ((*self_item) == this)
		{
			parent->childs.erase(self_item);
			break;
		}
		self_item++;
	}
}

void GameObject::RecursiveDeleteGameObject()
{
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->ComponentCleanUp();
		RELEASE((*item));
		item = components.erase(item);
	}

	std::list<GameObject*>::iterator g_item = childs.begin();

	while (g_item != childs.end() || childs.size()>0)
	{
		(*g_item)->RecursiveDeleteGameObject();
		RELEASE(*g_item);
		g_item = childs.erase(g_item);
	}

	if (App->level->selected_game_object = this)
	{
		App->level->selected_game_object = nullptr;
	}

	std::list<GameObject*>::iterator _item = App->level->game_objects.begin();

	while (_item != App->level->game_objects.end())
	{
		if ((*_item) == this)
		{
			App->level->game_objects.erase(_item);
			break;
		}
		_item++;
	}



}

Component * GameObject::GetComponent(ComponentType type)
{
	Component* ret;

	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		if ((*item)->component_type == type)
			ret = (*item);
	}

	return ret;
}
