#include "GUIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleImgui.h"
#include "ModuleCamera3D.h"



GUIScene::GUIScene(bool active = false) : GUIElements("scene",active)
{
	position = { 420, 200 };
	size = { 600, 300 };
}


GUIScene::~GUIScene()
{
}

void GUIScene::Draw()
{

	ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoNav);
	
		Move();
		ImGui::BeginChild("", ImGui::GetWindowSize(), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking|ImGuiWindowFlags_NoNav);
		if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(0))
		{
			App->camera->scene_clicked = true;
			LOG("clicked");
		}
		else
			App->camera->scene_clicked = false;

		ImGui::SetCursorPos({ -(App->window->window_w - size.x) / 2,-(App->window->window_h - size.y) / 2 });
		hovered = ImGui::IsMouseHoveringWindow();
		ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, { (float)App->window->window_w,(float)App->window->window_h }, { 0,1 }, { 1,0 });
		ImGui::EndChild();
		ImGui::End();
	
}

bool GUIScene::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	Node.AddMember("active", active, allocator);
	return true;
}
bool GUIScene::Load(rapidjson::Value& Node)
{
	active = Node["active"].GetBool();
	return true;
}
bool GUIScene::IsMouseHovering()
{
	return hovered;
}
