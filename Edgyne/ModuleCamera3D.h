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

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);

	void Look(const math::vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);
	float4x4* GetViewMatrix();

	
private:

	void CalculateViewMatrix();

public:
	
	vec X, Y, Z, Position, Reference;
	float4x4 Camera_view;
	
private:
	float4x4 ViewMatrix, ViewMatrixInverse;



};