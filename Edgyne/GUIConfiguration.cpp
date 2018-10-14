#include "GUIConfiguration.h"
#include "Application.h"


GUIConfiguration::GUIConfiguration() : GUIElements("Configuration")
{
	position = { 420, 200 };
	size = { 600, 300 };

}


GUIConfiguration::~GUIConfiguration()
{
}

void GUIConfiguration::Draw()
{
	if (ImGui::Begin("Configuration", &active))
	{
		Move();
		if (ImGui::CollapsingHeader("General Options"))
		{
			ImGui::Button("Default Options");
			ImGui::SameLine();
			if(ImGui::Button("Load Options"))
			{
				App->toLoad = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Save Options"))
			{
				App->toSave = true;
			}
		}

		App->Configuration_ImGui();
		std::list<Module*>::iterator item = App->list_modules.begin();


		while (item != App->list_modules.end())
		{
			(*item)->Configuration();
			item++;
		}
		App->Hardware_ImGui();
		ImGui::End();
	}

}

bool GUIConfiguration::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	Node.AddMember("active", active, allocator);

	return true;
}
