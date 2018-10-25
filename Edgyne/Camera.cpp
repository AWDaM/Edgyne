#include "Camera.h"



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

	Position = vec(5.0f, 5.0f, 5.0f);
	Reference = vec(0.0f, 0.0f, 0.0f);

	frustum.SetPerspective(DegToRad(106.67), DegToRad(60));
	frustum.SetViewPlaneDistances(0.5, 500);

	frustum.SetUp({ 0,1,0 });
	frustum.SetPos({ 0,0,0 });
	frustum.SetWorldMatrix(float3x4::identity);
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.Translate(Position);
	frustum.ComputeProjectionMatrix();
	CalculateViewMatrix();
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
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void Camera::LookAt(const vec & Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}

void Camera::Move(const vec & Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}



float * Camera::GetViewMatrix()
{
	return  &ViewMatrix[0][0];;
}

void Camera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}
