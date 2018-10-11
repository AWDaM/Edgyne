#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
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

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	void CameraAdaptation(vec3 new_pos, vec3 new_ref);
	float* GetViewMatrix();

	
private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	mat4x4 ViewMatrix, ViewMatrixInverse;
	
private:
	



};