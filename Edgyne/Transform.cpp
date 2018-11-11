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
	rotation_euler = { 0,0,0 };
	previous_rotation_euler = { 0,0,0 };
}


Transform::~Transform()
{
}

void Transform::TransformChanged()
{
	rotation = rotation.FromEulerXYZ(DEGTORAD*rotation_euler.x, DEGTORAD*rotation_euler.y, DEGTORAD*rotation_euler.z);
	//rotation = rotation.FromEulerXYZ(DEGTORAD*(rotation_euler.x-previous_rotation_euler.x), DEGTORAD*(rotation_euler.y-previous_rotation_euler.y), DEGTORAD*(rotation_euler.z-previous_rotation_euler.z));
	previous_rotation_euler = rotation_euler;
}

void Transform::OnEditor()
{
	if (ImGui::TreeNode("Transform"))
	{	
		if (!game_object->Static)
		{
			if (ImGui::InputFloat("Position X", &position.x, 1.0f, 10.0f, 6))
				game_object->transform_changed = true;
			if (ImGui::InputFloat("Position Y", &position.y, 1.0f, 10.0f, 6))
				game_object->transform_changed = true;
			if (ImGui::InputFloat("Position Z", &position.z, 1.0f, 10.0f, 6))
				game_object->transform_changed = true;

			ImGui::Dummy({ 10,10 });

			if (ImGui::InputFloat("Rotation X", &rotation_euler.x, .1f, 1.0f, 6))
				game_object->transform_changed = true;
			if (ImGui::InputFloat("Rotation Y", &rotation_euler.y, 0.1f, 1.0f, 6))
				game_object->transform_changed = true;
			if (ImGui::InputFloat("Rotation Z", &rotation_euler.z, 0.1f, 1.0f, 6))
				game_object->transform_changed = true;

			ImGui::Dummy({ 10,10 });

			if (ImGui::InputFloat("Scale X", &scale.x, 1.0f, 10.0f, 6))
				game_object->transform_changed = true;
			if (ImGui::InputFloat("Scale Y", &scale.y, 1.0f, 10.0f, 6))
				game_object->transform_changed = true;
			if (ImGui::InputFloat("Scale Z", &scale.z, 1.0f, 10.0f, 6))
				game_object->transform_changed = true;
		}

		else
		{
			ImGui::Text("Position X: %f", position.x);
			ImGui::Text("Position Y: %f", position.y);
			ImGui::Text("Position Z: %f", position.z);

			ImGui::Dummy({ 10,10 });

			ImGui::Text("Rotation X: %f", rotation_euler.x);
			ImGui::Text("Rotation Y: %f", rotation_euler.y);
			ImGui::Text("Rotation Z: %f", rotation_euler.z);

			ImGui::Dummy({ 10,10 });

			ImGui::Text("Scale X: %f", scale.x);
			ImGui::Text("Scale Y: %f", scale.y);
			ImGui::Text("Scale Z: %f", scale.z);
		}

		ImGui::TreePop();
	}
}

rapidjson::Value Transform::SaveToScene(rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value myData(rapidjson::kObjectType);
	myData.AddMember("UID", UID, allocator);
	myData.AddMember("Type", component_type, allocator);

	rapidjson::Value pos(rapidjson::kObjectType);

	pos.AddMember("x", position.x, allocator);
	pos.AddMember("y", position.y, allocator);
	pos.AddMember("z", position.z, allocator);
	myData.AddMember("Position", pos, allocator);

	rapidjson::Value rot(rapidjson::kObjectType);

	rot.AddMember("x", rotation.x, allocator);
	rot.AddMember("y", rotation.y, allocator);
	rot.AddMember("z", rotation.z, allocator);
	rot.AddMember("w", rotation.w, allocator);
	myData.AddMember("Rotation", rot, allocator);

	rapidjson::Value eul(rapidjson::kObjectType);

	eul.AddMember("x", rotation_euler.x, allocator);
	eul.AddMember("y", rotation_euler.y, allocator);
	eul.AddMember("z", rotation_euler.z, allocator);
	myData.AddMember("Rotation Euler", eul, allocator);

	rapidjson::Value peul(rapidjson::kObjectType);


	peul.AddMember("x", previous_rotation_euler.x, allocator);
	peul.AddMember("y", previous_rotation_euler.y, allocator);
	peul.AddMember("z", previous_rotation_euler.z, allocator);
	myData.AddMember("Previous Rotation Euler", peul, allocator);

	rapidjson::Value sc(rapidjson::kObjectType);


	sc.AddMember("x", scale.x, allocator);
	sc.AddMember("y", scale.y, allocator);
	sc.AddMember("z", scale.z, allocator);
	myData.AddMember("Scale", sc, allocator);

	return myData;
}
