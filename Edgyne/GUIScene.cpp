#include "GUIScene.h"
#include "Application.h"
#include "ModuleRenderer3D.h"



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
	ImGui::Begin("Scene", &active);

	ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, ImGui::GetWindowSize());
	ImGui::End();
}
