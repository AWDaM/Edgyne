#include "GUIShaders.h"



GUIShaders::GUIShaders(bool active = false) : GUIElements("Shaders", active)
{
}


GUIShaders::~GUIShaders()
{
}

void GUIShaders::Draw()
{
	ImGui::Begin("Shaders");
	ImGui::End();
}
