#include "Transform.h"
#include "Application.h"


Transform::Transform()
{
}

Transform::Transform(GameObject * game_object) : Component(game_object,TRANSFORM)
{
	position = { 0,0,0 };
	scale = { 1,1,1 };
	rotation = rotation.FromEulerXYZ(0, 0, 0);
}


Transform::~Transform()
{
}

void Transform::OnEditor()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::InputFloat("Position X", &position.x, 1.0f, 10.0f, 0.01f);
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
