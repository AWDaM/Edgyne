#ifndef __MODULE_CAMERA_H__
#define __MODULE_CAMERA_H__


#include "Module.h"
#include "Globals.h"
#include <vector>

class Timer;
class Camera;
class GameObject;

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

	LineSegment getMouseClickRay();
	void CameraAdaptation(vec new_pos, vec new_ref);

	
private:

public:
	bool scene_clicked = false;

	Camera* editor_camera;
};

#endif // !__MODULE_CAMERA_H__