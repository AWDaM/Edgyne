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
	if (parent)
	{
		UID = pcg32_random_r(&App->rng);
		parentUID = parent->UID;
		parent->childrenUID.push_back(UID);
	}
	else
	{
		UID = 1;
		parentUID = 0;
	}
	aligned_bounding_box.SetNegativeInfinity();
	name = _name;
	global_transform_matrix.SetIdentity();
}


GameObject::~GameObject()
{
	parent = nullptr;
	transform = nullptr;

	std::vector<Component*>::iterator it;
	it = components.begin();
	while (it != components.end())
	{
		RELEASE((*it));
		it++;
	}
	components.clear();

	std::list<GameObject*>::iterator item;
	item = childs.begin();
	while (item != childs.end())
	{
		RELEASE((*item));
		item++;
	}
	childs.clear();
}

bool GameObject::Update()
{
	UpdateComponents();
	

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
	return true;
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

void GameObject::SaveToScene(rapidjson::Value& obj, rapidjson::Document::AllocatorType& al)
{
	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		switch ((*item)->component_type)
		{
		case ComponentType::TRANSFORM:
			obj.AddMember("Transform",	(*item)->SaveToScene(al), al); break;
		case ComponentType::MESH:
			obj.AddMember("Mesh",		(*item)->SaveToScene(al), al); break;		
		case ComponentType::MATERIAL:
			obj.AddMember("Material",	(*item)->SaveToScene(al), al); break;
		case ComponentType::CAMERA:
			obj.AddMember("Camera",		(*item)->SaveToScene(al), al); break;
		default:
			break;
		}
		item++;
	}


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

void GameObject::RecursiveSetChildsStatic(bool _static)
{
	Static = _static;
	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->RecursiveSetChildsStatic(_static);
		item++;
	}
}

void GameObject::RecursiveSetChildsTransformChanged(bool _transform_changed)
{
	transform_changed = _transform_changed;
	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->RecursiveSetChildsTransformChanged(_transform_changed);
		item++;
	}

}

void GameObject::SetLocalBoundingVolume()
{
	aligned_bounding_box.SetNegativeInfinity();

	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		(*item)->SetBoundingVolume();
		item++;
	}

	aligned_bounding_box.TransformAsAABB(global_transform_matrix);


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

void GameObject::RecursiveTransformChanged(const float4x4 & parent)
{
	if (transform_changed)
	{
		global_transform_matrix = parent * float4x4::FromTRS(transform->position, transform->rotation, transform->scale);

		std::vector<Component*>::iterator item = components.begin();

		while (item != components.end())
		{
			(*item)->TransformChanged();
			item++;
		}
		SetLocalBoundingVolume();
		transform_changed = false;
		transform->previous_rotation_euler = transform->rotation_euler;
	}
	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->RecursiveTransformChanged(global_transform_matrix);
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
	std::vector<uint> tmp;
	for (std::vector<uint>::iterator it = parent->childrenUID.begin(); it != parent->childrenUID.end(); it++)
	{
		if ((*it) != UID)
		{
			tmp.push_back((*it));
		}
	}
	parent->childrenUID = tmp;
}

void GameObject::RecursiveDeleteGameObject()
{
	if (this != App->level->root)
	{
		std::vector<Component*>::iterator item = components.begin();

		while (item != components.end())
		{
			(*item)->ComponentCleanUp();
			RELEASE((*item));
			item = components.erase(item);
		}
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

	if (this != App->level->root)
	{
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
	if (this != App->level->root)
	{
	std::vector<uint> tmp;
	for (std::vector<uint>::iterator it = parent->childrenUID.begin(); it != parent->childrenUID.end(); it++)
	{
		if ((*it) != UID)
		{
			tmp.push_back((*it));
		}
	}
	parent->childrenUID = tmp;

	}
}

void GameObject::RecursiveResetAddedToQuadTree()
{
	added_to_quadtree_buffer = false;
	std::list<GameObject*>::iterator item = childs.begin();

	while (item != childs.end())
	{
		(*item)->RecursiveResetAddedToQuadTree();
		item++;
	}

}

Component * GameObject::GetComponent(ComponentType type)
{
	Component* ret = nullptr;

	std::vector<Component*>::iterator item = components.begin();

	while (item != components.end())
	{
		if ((*item)->component_type == type)
			ret = (*item);
		item++;
	}

	return ret;
}

void GameObject::LinkMeshWithMaterial()
{
	Mesh* mesh = (Mesh*)GetComponent(MESH);
	if (mesh)
	{
		Material* material = (Material*)GetComponent(MATERIAL);
		if (material)
			mesh->material = material;
	}
}
