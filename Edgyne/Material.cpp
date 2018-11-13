#include "Material.h"
#include "Application.h"

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


	rapidjson::Value img(rapidjson::kObjectType);

	img.AddMember("Width", img_size.x, allocator);
	img.AddMember("Height", img_size.y, allocator);
	myData.AddMember("Size", img, allocator);

	return myData;
}

void Material::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["UID"].GetUint();
	fileName = comp->value["Texture Name"].GetString();


	img_size.Set(comp->value["Size"]["Width"].GetFloat(), comp->value["Size"]["Height"].GetFloat());

	color.Set(comp->value["Color"]["r"].GetFloat(), comp->value["Color"]["g"].GetFloat(), comp->value["Color"]["b"].GetFloat());


}

bool Material::ComponentDraw()
{
	return true;

}

void Material::MaterialBind()
{
	if (id_texture)
	{
		glBindTexture(GL_TEXTURE_2D, id_texture);
	}

	else
		glColor3f(color.x, color.y, color.z);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Material::MaterialUnbind()
{
	if (id_texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		glColor3f(1, 1, 1);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
