#include "GUIConfiguration.h"
#include "Application.h"


GUIConfiguration::GUIConfiguration() : GUIElements("Configuration")
{
}


GUIConfiguration::~GUIConfiguration()
{
}

void GUIConfiguration::Draw()
{
	if (ImGui::Begin("Configuration", &active))
	{
		if (ImGui::CollapsingHeader("General Options"))
		{
			ImGui::Button("Default Options");
			ImGui::SameLine();
			ImGui::Button("Load Options");
			ImGui::SameLine();
			ImGui::Button("Save Options");
		}

		App->Configuration_ImGui();
		std::list<Module*>::iterator item = App->list_modules.begin();


		while (item != App->list_modules.end())
		{
			(*item)->Configuration();
			item++;
		}

	}
	App->Hardware_ImGui();
	ImGui::End();
}
