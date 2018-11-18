#include "GUIAssets.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleLoader.h"
#include <experimental/filesystem>


GUIAssets::GUIAssets(bool active = false) : GUIElements("assets", active)
{
}


GUIAssets::~GUIAssets()
{
}

void GUIAssets::Draw()
{
	ImGui::Begin("Assets", &active);

	if (currentPath.c_str() != "Assets\\")
	{
		if (ImGui::Button("Root"))
		{
			currentPath = "Assets\\";
		}
	}
	const std::experimental::filesystem::directory_iterator end{};

	std::string button_name;
	for (std::experimental::filesystem::directory_iterator iter{ currentPath.c_str() }; iter != end; ++iter)
	{
		if (std::experimental::filesystem::is_directory(*iter))
		{
			if (ImGui::Button(iter->path().string().c_str()))
			{
				currentPath = iter->path().string();
			}
		}
		else if (std::experimental::filesystem::is_regular_file(*iter))
		{
			button_name = iter->path().string();
			button_name = button_name.erase(0, button_name.find_last_of("\\") + 1);

			if (ImGui::Button(button_name.c_str()))
			{
				std::string fileName = iter->path().string();
				fileName = fileName.erase(0, fileName.find_last_of("\\") + 1);
				fileName = fileName.substr(0, fileName.find("."));

				std::string extension = iter->path().string();
				extension = extension.erase(0, extension.find("."));
				if (extension == App->importer->modelExtension)
				{
					App->loader->LoadObject(fileName, false);
				}
				else if (extension == App->importer->sceneExtension)
				{
					App->loader->LoadObject(fileName, true);
				}

			}
		}
	}

	ImGui::End();

}

bool GUIAssets::Load(rapidjson::Value & Node)
{
	return false;
}

bool GUIAssets::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType & allocator)
{
	return false;
}
