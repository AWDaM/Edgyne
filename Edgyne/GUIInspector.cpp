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
		if(ImGui::TreeNode((*item)->name))
		{
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::Text("Position X: %f",(*item)->position.x);
				ImGui::Text("Position Y: %f", (*item)->position.y);
				ImGui::Text("Position Z: %f", (*item)->position.z);
				ImGui::Dummy({ 10,10 });
				ImGui::Text("Rotation X: %f", (*item)->rotation.x);
				ImGui::Text("Rotation Y: %f", (*item)->rotation.y);
				ImGui::Text("Rotation Z: %f", (*item)->rotation.z);
				ImGui::Dummy({ 10,10 });
				ImGui::Text("Scale X: %f", (*item)->scale.x);
				ImGui::Text("Scale Y: %f", (*item)->scale.y);
				ImGui::Text("Scale Z: %f", (*item)->scale.z);
			
				ImGui::TreePop();
			}
			if(ImGui::TreeNode("Mesh"))
			{
				ImGui::Text("Num Faces: %i", (*item)->num_faces);
				ImGui::Text("Num Vertices: %i", (*item)->num_vertex);
				ImGui::Text("Num Indexes: %i", (*item)->num_index);
				ImGui::TreePop();
			}
			if(ImGui::TreeNode("Texture"))
			{
				ImGui::Image((void *)(intptr_t)(*item)->id_texture, ImVec2(512, 512));
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}


		item++;
	}
	
	ImGui::End();
}
