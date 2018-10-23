#include "GUIHierarchy.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "GameObject.h"



GUIHierarchy::GUIHierarchy(bool active = false) : GUIElements("hierarchy", active)
{
}

GUIHierarchy::~GUIHierarchy()
{
}

void GUIHierarchy::Draw()
{
	ImGui::Begin("Hierarchy", &active);
	App->level->root->OnHierarchy();
	ImGui::End();
}

bool GUIHierarchy::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType & allocator)
{
	return false;
}

bool GUIHierarchy::Load(rapidjson::Value & Node)
{
	return false;
}
