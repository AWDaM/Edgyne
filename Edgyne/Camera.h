#pragma once
#include "Component.h"


class Camera :
	public Component
{
public:
	Camera();
	~Camera();

	bool ComponentStart();
	bool ComponentUpdate();
	bool ComponentCleanUp();

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);

	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:

private:
	Frustum frustum;
	vec X, Y, Z, Position, Reference;
	float4x4 ViewMatrix, ViewMatrixInverse;
};

