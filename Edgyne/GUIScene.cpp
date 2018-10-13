#include "GUIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"



GUIScene::GUIScene() : GUIElements("scene")
{
	posx = 420;
	posy = 200;
	width = 600;
	height = 300;
	active = true;
}


GUIScene::~GUIScene()
{
}

void GUIScene::Draw()
{
	ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	hovered = ImGui::IsMouseHoveringWindow();
	ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, { (float)App->window->window_w,(float)App->window->window_h }, { 0,1 }, { 1,0 });
	ImGui::End();
}

bool GUIScene::IsMouseHovering()
{
	return hovered;
}
