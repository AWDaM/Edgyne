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
	ImGui::Begin("Scene", &active);

	ImGui::Image((ImTextureID)App->renderer3D->framebuffer_texture, { (float)App->window->window_w,(float)App->window->window_h }, { 0,1 }, { 1,0 });
	ImGui::End();
}

bool GUIScene::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	Node.AddMember("posx", posx, allocator);
	Node.AddMember("posy", posy, allocator);
	Node.AddMember("width", width, allocator);
	Node.AddMember("height", height, allocator);
	Node.AddMember("active", active, allocator);
	return true;
}
