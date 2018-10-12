#include "GUIInspector.h"



GUIInspector::GUIInspector() : GUIElements("inspector")
{
}


GUIInspector::~GUIInspector()
{
}

void GUIInspector::Draw()
{
	ImGui::Begin("Inspector", &active);
	ImGui::CollapsingHeader("Transform");
	ImGui::CollapsingHeader("Mesh");
	ImGui::CollapsingHeader("Texture");
	ImGui::End();
}
