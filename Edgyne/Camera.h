#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "Component.h"


class Camera :
	public Component
{
public:
	Camera(GameObject* game_object);
	~Camera();

	bool ComponentStart();
	bool ComponentUpdate();
	bool ComponentCleanUp();

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);

	float* GetProjectionMatrix();
	float* GetViewMatrix();
	void CalculateViewMatrix();

	void TransformChanged();
	void OnEditor();

	void ChangeFrustum();

private:



public:
	Frustum frustum;
	vec X, Y, Z, Position, Reference;
	float4x4 ViewMatrix, ViewMatrixInverse;
	float near_plane_distance, far_plane_distance, horizontal_fov, vertical_fov, aspect_ratio;

private:


};

#endif // !__CAMERA__