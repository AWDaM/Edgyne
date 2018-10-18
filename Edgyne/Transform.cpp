#include "Transform.h"
#include "Application.h"


Transform::Transform()
{
}

Transform::Transform(GameObject * game_object) : Component(game_object,TRANSFORM)
{
}


Transform::~Transform()
{
}

void Transform::OnEditor()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::Text("Position X: %f", position.x);
		ImGui::Text("Position Y: %f", position.y);
		ImGui::Text("Position Z: %f", position.z);
		ImGui::Dummy({ 10,10 });
		ImGui::Text("Rotation X: %f", rotation.x);
		ImGui::Text("Rotation Y: %f", rotation.y);
		ImGui::Text("Rotation Z: %f", rotation.z);
		ImGui::Dummy({ 10,10 });
		ImGui::Text("Scale X: %f", scale.x);
		ImGui::Text("Scale Y: %f", scale.y);
		ImGui::Text("Scale Z: %f", scale.z);

		ImGui::TreePop();
	}
}
