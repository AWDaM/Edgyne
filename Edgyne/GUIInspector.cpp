#include "GUIInspector.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleLevel.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Component.h"
#include "QuadTree.h"


#include <list>
#include <experimental/filesystem>


GUIInspector::GUIInspector(bool active = false) : GUIElements("inspector",active)
{
	position = { 420, 200 };
	size = { 600, 300 };
}


GUIInspector::~GUIInspector()
{
	highlighted_game_object = nullptr;
}

void GUIInspector::Draw()
{
	ImGui::Begin("Inspector", &active);
		Move();
	
		if (App->level->selected_game_object)
		{
			static char name[25];
			strcpy_s(name, 25, App->level->selected_game_object->name.c_str());
			if (ImGui::InputText("Name", name, 25, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				App->level->selected_game_object->name = name;
			}
			if (ImGui::Checkbox("Active", &App->level->selected_game_object->active))
			{
				App->level->selected_game_object->RecursiveSetChildsActive(App->level->selected_game_object->active);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Static", &App->level->selected_game_object->Static))
			{
				App->level->selected_game_object->RecursiveSetChildsStatic(App->level->selected_game_object->Static);
				App->level->generate_quadtree = true;
				//App->level->quad_tree->Clear
			}
			ImGui::Checkbox("Delete", &App->level->selected_game_object->to_remove);

			std::vector<Component*>::iterator item = App->level->selected_game_object->components.begin();

			while (item != App->level->selected_game_object->components.end())
			{
				(*item)->OnEditor();
				item++;
			}

			if (ImGui::CollapsingHeader("Add Component"))
			{
				AddComponents();
			}
		}
		
		ImGui::End();
}

bool GUIInspector::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	Node.AddMember("active", active, allocator);

	return true;
}
void GUIInspector::AddComponents()
{
	if (ImGui::Button("Transform"))
	{
		if (!App->level->selected_game_object->GetComponent(TRANSFORM))
			App->level->selected_game_object->AddComponent(TRANSFORM);
		else
			LOG("The gameobject already has a TRANSFORM component");
	}
	if (ImGui::Button("Camera"))
	{
		if (!App->level->selected_game_object->GetComponent(CAMERA))
			App->level->selected_game_object->AddComponent(CAMERA);
		else
			LOG("The gameobject already has a CAMERA component");
	}
	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (!App->level->selected_game_object->GetComponent(MESH))
		{
			AddMesh();
		}

	}
}
void GUIInspector::AddMesh()
{
	const std::experimental::filesystem::directory_iterator end{};

	for (std::experimental::filesystem::directory_iterator iter{"Library\\Meshes\\" }; iter != end; ++iter)
	{
		std::string extension = iter->path().string();
		extension = extension.erase(0, extension.find("."));

		std::string fileName = iter->path().string();
		fileName = fileName.erase(0, fileName.find_last_of("\\") + 1);

		if (App->importer->meshExtension == extension)
		{
			if (ImGui::Button(fileName.c_str()))
			{
				Mesh* mesh = (Mesh*)App->level->selected_game_object->AddComponent(MESH);
				ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->GetResourceFromUID(fileName);
				if (!resource_mesh)
				{
					resource_mesh = (ResourceMesh*)App->resource_manager->CreateNewResource(Resource::RESOURCE_MESH, fileName);
				}
				mesh->resource_mesh = resource_mesh->file;
			}
		}
	}
}
bool GUIInspector::Load(rapidjson::Value& Node)
{
	active = Node["active"].GetBool();
	return true;
}