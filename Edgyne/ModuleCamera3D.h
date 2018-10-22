#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class Timer;


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(rapidjson::Value& node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);

	void CameraAdaptation(vec new_pos, vec new_ref);
	float* GetViewMatrix();

	
private:

	void CalculateViewMatrix();

public:
	
	vec X, Y, Z, Position, Reference;
	float4x4 ViewMatrix, ViewMatrixInverse;
};