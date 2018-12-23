#include "Material.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleLoader.h"
#include "ModuleResourceManager.h"
#include "ResourceMaterial.h"
#include "Resource.h"

#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



Material::Material()
{
}

Material::Material(GameObject * game_object) : Component(game_object,MATERIAL)
{
}


Material::~Material()
{
}

rapidjson::Value Material::SaveToScene(rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value myData(rapidjson::kObjectType);

	myData.AddMember("UID", UID, allocator);
	myData.AddMember("Type", component_type, allocator);
	myData.AddMember("Resource UID", (rapidjson::Value::StringRefType)resource_uid.c_str(), allocator);

	return myData;
}

void Material::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["UID"].GetUint();
	resource_uid = comp->value["Resource UID"].GetString();
	if (!App->resource_manager->GetResourceFromUID(resource_uid))
	{
		App->resource_manager->CreateNewResource(Resource::ResourceType::RES_MATERIAL, resource_uid);
	}
}


void Material::LoadAsMeshComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["Material"]["UID"].GetUint();
	resource_uid = comp->value["Material"]["Resource UID"].GetString();
	if (!App->resource_manager->GetResourceFromUID(resource_uid))
	{
		App->resource_manager->CreateNewResource(Resource::ResourceType::RES_MATERIAL, resource_uid);
	}
}


bool Material::ComponentDraw()
{
	return true;

}


void Material::OnEditor()
{
	ResourceMaterial* material = (ResourceMaterial*)App->resource_manager->GetResourceFromUID(resource_uid);
	if (material)
	{
		if (ImGui::TreeNode("Material"))
		{
			if (material->id_texture > 0)
			{
				ImGui::Text("Image size %ix%i", (int)material->img_size.x, (int)material->img_size.y);
				ImGui::Image((void *)(intptr_t)material->id_texture, ImVec2(256, 256));
			}

			ImGui::InputFloat4("Color", material->color.ptr(), 100);

			ImGui::Text("Shader Program:");
			ImGui::SameLine();
			if(ImGui::CollapsingHeader(material->shaderProgram.c_str()))
			{
				for (std::map<std::string, Resource*>::iterator it = App->resource_manager->resources.begin(); it != App->resource_manager->resources.end(); ++it)
				{
					if ((*it).second->type == Resource::RES_SHADER)
					{
						if (ImGui::Button((*it).first.c_str()))
						{
							material->shaderProgram = (*it).first.c_str();
						}
					}
				}
				if (ImGui::Button("Create New Program"))
				{

				}
			}
			if (ImGui::CollapsingHeader("Edit Program"))
			{
				ImGui::Button("AddExistingShader");
				if (ImGui::Button("AddNewShader"))
				{
					ImGui::OpenPopup("Shader Name");
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginPopupModal("Shader Name"))
			{
				static char name[25];
				if (ImGui::InputText("", name, 25, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				{
				
				}
				ImGui::EndPopup();
			}

			ImGui::TreePop();
		}
	}
}
