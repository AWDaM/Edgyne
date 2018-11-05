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
	return false;
}

void GameObject::OnHierarchy(int id)
{
	std::vector<GameObject*>::iterator item = childs.begin();
	while (item != childs.end())
	{
		if (!(*item)->active)
		{
			ImGui::PushStyleColor(1, { 128,128,128,128 });
		}
		id++;
		ImGui::PushID(id);
		if (ImGui::TreeNode((*item)->name.c_str()))
		{
			if(ImGui::IsItemClicked())
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

	std::vector<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->CalcGlobalTransform(global_transform_matrix);
		item++;
	}
}
