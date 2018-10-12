#include "GUIInspector.h"
#include "Application.h"
#include "ModuleRenderer3D.h"


#include <list>

GUIInspector::GUIInspector() : GUIElements("inspector")
{
}


GUIInspector::~GUIInspector()
{
	
}

void GUIInspector::Draw()
{
	ImGui::Begin("Inspector", &active);

	std::list<mesh*>::iterator item = App->renderer3D->mesh_list.begin();

	while (item != App->renderer3D->mesh_list.end())
	{
		ImGui::CollapsingHeader((*item)->name);
		{
			ImGui::CollapsingHeader("Transform");
			ImGui::CollapsingHeader("Mesh");
			ImGui::CollapsingHeader("Texture");
		}
		item++;
	}
	
	ImGui::End();
}
