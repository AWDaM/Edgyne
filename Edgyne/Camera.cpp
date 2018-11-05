#include "Camera.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleLevel.h"



Camera::Camera(GameObject* game_object) : Component(game_object,CAMERA)
{
	
}


Camera::~Camera()
{
}

bool Camera::ComponentStart()
{

	X = vec(1.0f, 0.0f, 0.0f);
	Y = vec(0.0f, 1.0f, 0.0f);
	Z = vec(0.0f, 0.0f, 1.0f);

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
	horizontal_fov = 106.67;
	vertical_fov = 60;

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

void Camera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

void Camera::TransformChanged()
{
	frustum.pos = game_object->transform->position;
	Quat rotation = Quat(game_object->transform->rotation);
	frustum.front = rotation.Mul(frustum.front).Normalized();
	frustum.up = rotation.Mul(frustum.up).Normalized();
}

void Camera::SetBoundingVolume()
{
	game_object->aligned_bounding_box.Enclose(frustum);
	//for (int i = 0; i < num_vertex*3; i+3)
	//{
	//	game_object->bounding_box.Enclose((float3)vertex[i]);
	//}

	game_object->bounding_sphere.Enclose(frustum);
}

void Camera::OnEditor()
{
	if (ImGui::TreeNode("Camera"))
	{
		if (ImGui::SliderFloat("Near Plane Distance", &near_plane_distance, 0.1, 1000))
			ChangeFrustum();
		if(ImGui::SliderFloat("Far Plane Distance", &far_plane_distance, 0.2, 1001))
			ChangeFrustum();
		if(ImGui::SliderFloat("Horizontal  field of view", &horizontal_fov, 0.1, 180))
			ChangeFrustum();
		if(ImGui::SliderFloat("Vertical field of view", &vertical_fov, 0.1, 180))
			ChangeFrustum();
		ImGui::TreePop();
	}
}

void Camera::ChangeFrustum()
{
	//frustum.SetPerspective(DegToRad(horizontal_fov), DegToRad(vertical_fov));
	//frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;

	frustum.verticalFov = DEGTORAD * 90.0f;
	frustum.horizontalFov = 2.f * Atan(Tan(frustum.verticalFov*0.5f)*(16.0f / 9.0f));
}
