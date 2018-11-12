#include "GUIHierarchy.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "GameObject.h"



GUIHierarchy::GUIHierarchy(bool active = false) : GUIElements("hierarchy", active)
{
}

GUIHierarchy::~GUIHierarchy()
{
}

void GUIHierarchy::Draw()
{
	ImGui::Begin("Hierarchy", &active);
	if (ImGui::Button("Add New GameObject"))
	{
		if (App->level->selected_game_object == nullptr)
		{
			App->level->NewGameObject("GameObject");
		}
		else
		{
			App->level->selected_game_object->AddGameObject("GameObject");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("DELET THIS"))
	{
		App->level->root->to_remove = true;
	}
	int id = 0;
	App->level->root->OnHierarchy(id);
	ImGui::End();
}

bool GUIHierarchy::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType & allocator)
{
	return false;
}

bool GUIHierarchy::Load(rapidjson::Value & Node)
{
	return false;
}
