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
	myData.AddMember("Resource UID", resource_uid, allocator);

	return myData;
}

void Material::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["UID"].GetUint();
	resource_uid = comp->value["Resource UID"].GetUint();
	if (!App->resource_manager->GetResourceFromUID(resource_uid))
	{
		App->resource_manager->CreateResource(MATERIAL, resource_uid);
	}
}


void Material::LoadAsMeshComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["Material"]["UID"].GetUint();
	resource_uid = comp->value["Material"]["Resource UID"].GetUint();
	if (!App->resource_manager->GetResourceFromUID(resource_uid))
	{
		App->resource_manager->CreateResource(MATERIAL, resource_uid);
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

			ImGui::InputFloat3("Color", material->color.ptr(), 100);
			ImGui::TreePop();
		}
	}
}
