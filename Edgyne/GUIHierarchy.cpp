#include "GUIHierarchy.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "ModuleLoader.h"
#include "GameObject.h"



GUIHierarchy::GUIHierarchy(bool active = false) : GUIElements("hierarchy", active)
{
}

GUIHierarchy::~GUIHierarchy()
{
}

void GUIHierarchy::Draw()
{
	std::string title = App->level->current_scene_name.c_str();
	title.append("###");
	ImGui::Begin(title.c_str(), &active);

	if (ImGui::Button("Save Scene"))
	{
		App->loader->SaveObject(App->level->current_scene_name.c_str(),true,App->level->game_objects);
	}

	if(ImGui::CollapsingHeader("Save Scene As"))
	{
		static char name[25];
		strcpy_s(name, 25, App->level->current_scene_name.c_str());
		if (ImGui::InputText("Name", name, 25, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			if (!App->loader->CheckIfNameExists(name))
			{
				App->loader->SaveObject(name, true, App->level->game_objects);
				App->level->current_scene_name = name;
			}
			else
			{
				ImGui::OpenPopup("The name already exists");
			}
		}
	}
	

	if (ImGui::BeginPopupModal("The name already exists"))
	{
		if (ImGui::Button("Overwrite Scene"))
		{
			App->loader->SaveObject(name, true, App->level->game_objects);
		}
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	

		ImGui::SetNextWindowSize({ 277,55 });
		if (ImGui::BeginPopupModal("Do you want to save the Scene before?",NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			if (ImGui::Button("Yes"))
			{
				App->loader->SaveObject(App->level->current_scene_name.c_str(), true, App->level->game_objects);
				App->level->root->to_remove = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				App->level->root->to_remove = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

	
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

	if (ImGui::Button("Clear Scene"))
		ImGui::OpenPopup("Do you want to save the Scene before?");

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
