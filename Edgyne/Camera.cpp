#include "Camera.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "ModuleCamera3D.h"
#include "ModuleDebug.h"
#include "ModuleWindow.h"



Camera::Camera(GameObject* game_object) : Component(game_object,CAMERA)
{
	
}


Camera::~Camera()
{
}

bool Camera::ComponentStart()
{

	if (game_object == App->level->root)
	{
		Position = vec(-10.0f, 5.0f, 7.0f);
		Reference = vec(0.0f, 0.0f, -1.0f);
	}
	else
	{
		Position = game_object->transform->position;
		Reference = game_object->transform->position;
		Reference.x += 1;
		Reference.Normalize();
	}
	near_plane_distance = 0.1;
	far_plane_distance = 1000;
	vertical_fov = 60;
	aspect_ratio = (float)App->window->window_w/App->window->window_h;

	frustum.type = math::FrustumType::PerspectiveFrustum;
	frustum.pos = Position;
	frustum.front = Reference;
	frustum.up = { 0.0f,1.0f,0.0f };

	ChangeFrustum();

	LookAt(Reference);
	return true;
}

bool Camera::ComponentUpdate()
{
	return true;
}

bool Camera::ComponentCleanUp()
{
	return true;
}

bool Camera::ComponentDraw()
{
	if(App->camera->editor_camera != this)
		App->debug->Draw_Camera(this);
	return true;
}

rapidjson::Value Camera::SaveToScene(rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value myData(rapidjson::kObjectType);

	myData.AddMember("UID", UID, allocator);
	myData.AddMember("Type", component_type, allocator);

	myData.AddMember("Near Plane", near_plane_distance, allocator);
	myData.AddMember("Far Plane", far_plane_distance, allocator);
	myData.AddMember("Aspect Ratio", aspect_ratio, allocator);
	myData.AddMember("Horizontal FoV", horizontal_fov, allocator);
	myData.AddMember("Vertical FoV", vertical_fov, allocator);


	rapidjson::Value pos(rapidjson::kObjectType);

	pos.AddMember("x", Position.x, allocator);
	pos.AddMember("y", Position.y, allocator);
	pos.AddMember("z", Position.z, allocator);
	myData.AddMember("Position", pos, allocator);


	rapidjson::Value ref(rapidjson::kObjectType);

	ref.AddMember("x", Reference.x, allocator);
	ref.AddMember("y", Reference.y, allocator);
	ref.AddMember("z", Reference.z, allocator);
	myData.AddMember("Reference", ref, allocator);

	return myData;
}

void Camera::LoadComponent(rapidjson::Value::ConstMemberIterator comp)
{
	UID = comp->value["UID"].GetUint();
	near_plane_distance = comp->value["Near Plane"].GetFloat();
	far_plane_distance = comp->value["Far Plane"].GetFloat();
	aspect_ratio = comp->value["Aspect Ratio"].GetFloat();
	horizontal_fov = comp->value["Horizontal FoV"].GetFloat();
	vertical_fov = comp->value["Vertical FoV"].GetFloat();

	Position.Set(comp->value["Position"]["x"].GetFloat(), comp->value["Position"]["y"].GetFloat(), comp->value["Position"]["z"].GetFloat());

	Reference.Set(comp->value["Reference"]["x"].GetFloat(), comp->value["Reference"]["y"].GetFloat(), comp->value["Reference"]["z"].GetFloat());


	frustum.type = math::FrustumType::PerspectiveFrustum;
	frustum.pos = Position;
	frustum.front = Reference;
	frustum.up = { 0.0f,1.0f,0.0f };

	ChangeFrustum();

	LookAt(Reference);
}

void Camera::Look(const vec & Position, const vec & Reference, bool RotateAroundReference)
{
	
}

void Camera::LookAt(const vec& Spot)
{
	Reference = Spot;

	vec front = Reference - frustum.pos;

	float3x3 look = float3x3::LookAt(frustum.front.Normalized(), front.Normalized(), frustum.up.Normalized(), { 0,1,0 });

	frustum.front = look.MulDir(frustum.front).Normalized();
	frustum.up = look.MulDir(frustum.up).Normalized();
}

void Camera::Move(const vec & Movement)
{
	Position += Movement;
	Reference += Movement;
	frustum.pos += Movement;
}



float * Camera::GetProjectionMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();
	return (float*)matrix.v;
}

float * Camera::GetViewMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();
	return  (float*)matrix.v;
}


void Camera::TransformChanged()
{

	frustum.pos = game_object->transform->position;
	Quat rotation = rotation.FromEulerXYZ(DEGTORAD*(game_object->transform->rotation_euler.x - game_object->transform->previous_rotation_euler.x), DEGTORAD*(game_object->transform->rotation_euler.y - game_object->transform->previous_rotation_euler.y), DEGTORAD*(game_object->transform->rotation_euler.z - game_object->transform->previous_rotation_euler.z));

	frustum.front = rotation.Mul(frustum.front).Normalized();

	frustum.up = rotation.Mul(frustum.up).Normalized();
}

void Camera::SetBoundingVolume()
{
	game_object->aligned_bounding_box.Enclose(frustum);
	game_object->aligned_bounding_box.Translate(-frustum.pos);

	game_object->bounding_sphere.Enclose(frustum);
	game_object->bounding_sphere.Translate(-frustum.pos);
}

void Camera::OnEditor()
{
	if (ImGui::TreeNode("Camera"))
	{
		if (ImGui::SliderFloat("Near Plane Distance", &near_plane_distance, 0.1, 1000))
			ChangeFrustum();
		if(ImGui::SliderFloat("Far Plane Distance", &far_plane_distance, 0.2, 1001))
			ChangeFrustum();
		if(ImGui::SliderFloat("Vertical field of view", &vertical_fov, 0.1, 180))
			ChangeFrustum();
		if (ImGui::SliderFloat("Aspect Ratio", &aspect_ratio, 0.1, 5))
			ChangeFrustum();
		ImGui::TreePop();
	}
}

void Camera::ChangeFrustum()
{
	//frustum.SetPerspective(DegToRad(horizontal_fov), DegToRad(vertical_fov));
	//frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);
	frustum.nearPlaneDistance = near_plane_distance;
	frustum.farPlaneDistance =far_plane_distance;

	frustum.verticalFov = DEGTORAD * vertical_fov;
	frustum.horizontalFov = 2.f * Atan(Tan(frustum.verticalFov*0.5f)*(aspect_ratio));
}
