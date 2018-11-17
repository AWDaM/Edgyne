#include "Material.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleLoader.h"

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
	myData.AddMember("Texture Name", (rapidjson::Value::StringRefType)fileName.c_str(), allocator);

	rapidjson::Value col(rapidjson::kObjectType);

	col.AddMember("r", color.x, allocator);
	col.AddMember("g", color.y, allocator);
	col.AddMember("b", color.z, allocator);
	myData.AddMember("Color", col, allocator);

	return myData;
}

void Material::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["UID"].GetUint();
	fileName = comp->value["Texture Name"].GetString();

	color.Set(comp->value["Color"]["r"].GetFloat(), comp->value["Color"]["g"].GetFloat(), comp->value["Color"]["b"].GetFloat());

	App->loader->LoadTextureFromLibrary(fileName.c_str(), this);

}
void Material::LoadAsMeshComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["Material"]["UID"].GetUint();
	fileName = comp->value["Material"]["Texture Name"].GetString();

	color.Set(comp->value["Material"]["Color"]["r"].GetFloat(), comp->value["Material"]["Color"]["g"].GetFloat(), comp->value["Material"]["Color"]["b"].GetFloat());

	App->loader->LoadTextureFromLibrary(fileName.c_str(), this);

}


bool Material::ComponentDraw()
{
	return true;

}


void Material::OnEditor()
{
	if (ImGui::TreeNode("Material"))
	{
		if (id_texture > 0)
		{
			ImGui::Text("Image size %ix%i", (int)img_size.x, (int)img_size.y);
			ImGui::Image((void *)(intptr_t)id_texture, ImVec2(256, 256));
		}
		
		ImGui::InputFloat3("Color", color.ptr(), 100);
		ImGui::TreePop();
	}

}
