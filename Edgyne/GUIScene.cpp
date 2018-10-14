#include "GUIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"



GUIScene::GUIScene() : GUIElements("scene")
{
	position = { 420, 200 };
	size = { 600, 300 };
	active = true;
}


GUIScene::~GUIScene()
{
}

void GUIScene::Draw()
{
	ImGuiWindowFlags_ window_flags;
	//if(ImGui::IsItemHovered)
	//ImGui::SetNextWindowContentSize({ (float)App->window->window_w,(float)App->window->window_h });
	if (ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		Move();
		ImGui::BeginChild("", ImGui::GetWindowSize(), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
		hovered = ImGui::IsMouseHoveringWindow();
		ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, { (float)App->window->window_w,(float)App->window->window_h }, { 0,1 }, { 1,0 });

		//ImGui::ImageButton((ImTextureID)App->renderer3D->framebuffer_texture, { (float)App->window->window_w,(float)App->window->window_h }, { 0,1 }, { 1,0 });
		ImGui::EndChild();
		ImGui::End();
	}
}

	bool GUIScene::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	Node.AddMember("active", active, allocator);
	return true;
}

bool GUIScene::IsMouseHovering()
{
	return hovered;
}
