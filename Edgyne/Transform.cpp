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
		if (ImGui::InputFloat("Position X", &position.x, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		if (ImGui::InputFloat("Position Y", &position.y, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		if (ImGui::InputFloat("Position Z", &position.z, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;

		ImGui::Dummy({ 10,10 });

		if (ImGui::InputFloat("Rotation X", &rotation.x, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		if (ImGui::InputFloat("Rotation Y", &rotation.y, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		if (ImGui::InputFloat("Rotation Z", &rotation.z, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		
		ImGui::Dummy({ 10,10 });

		if (ImGui::InputFloat("Scale X", &scale.x, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		if (ImGui::InputFloat("Scale Y", &scale.y, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;
		if (ImGui::InputFloat("Scale Z", &scale.z, 1.0f, 10.0f, 0.01f))
			game_object->transform_changed = true;

		ImGui::TreePop();
	}
}
