#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "ModuleLevel.h"
#include "Camera.h"
#include "GameObject.h"
#include "GUIScene.h"
#include "Globals.h"
#include "Timer.h"




ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Camera";
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init(rapidjson::Value& node)
{
	editor_camera = (Camera*)App->level->root->AddComponent(CAMERA);
	editor_camera->ComponentStart();
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	bool ret = true;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	editor_camera = nullptr;
	return true;
}

void ModuleCamera3D::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->game_state == RUNNING)
	{
		return UPDATE_CONTINUE;
	}
	if (App->imGui->scene->IsMouseHovering())
	{
		vec newPos(0.f, 0.f, 0.f);
		float speed = CAMERA_SPEED * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= CAMERA_SPEED_MULTIPLIER;
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				vec tmp = editor_camera->frustum.front.Normalized() * speed;
				tmp.y = 0;
				newPos += tmp;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
			{
				vec tmp = editor_camera->frustum.front.Normalized() * speed;
				tmp.y = 0;
				newPos -= tmp;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
			{
				vec tmp;
				tmp.x = editor_camera->frustum.front.Normalized().z;
				tmp.z = - editor_camera->frustum.front.Normalized().x;
				tmp.y = 0;
				newPos += tmp * speed;
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				vec tmp;
				tmp.x = - editor_camera->frustum.front.Normalized().z;
				tmp.z = editor_camera->frustum.front.Normalized().x;
				tmp.y = 0;
				newPos += tmp * speed;
			}
		}

		if (App->input->GetMouseZ() > 0)
		{
			newPos += editor_camera->frustum.front.Normalized() * speed * CAMERA_SPEED_MULTIPLIER;
		}
		else if (App->input->GetMouseZ() < 0)
		{
			newPos -= editor_camera->frustum.front.Normalized() * speed * CAMERA_SPEED_MULTIPLIER;
		}

		editor_camera->Move(newPos);

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			float dx = (float)App->input->GetMouseXMotion()*CAMERA_ROTATION_SPEED*dt*-1;
			float dy = (float)App->input->GetMouseYMotion()*CAMERA_ROTATION_SPEED*dt*-1;

			float Sensitivity = 0.25f;

			if (dx != 0)
			{
				Quat rotationX = Quat::RotateY(dx);
				editor_camera->frustum.front = rotationX.Mul(editor_camera->frustum.front).Normalized();
				editor_camera->frustum.up = rotationX.Mul(editor_camera->frustum.up).Normalized();
			}

			if (dy != 0)
			{
				Quat rotationY = Quat::RotateAxisAngle(editor_camera->frustum.WorldRight(), dy);

				float3 new_up = rotationY.Mul(editor_camera->frustum.up).Normalized();

				if (new_up.y > 0.0f)
				{
					editor_camera->frustum.up = new_up;
					editor_camera->frustum.front = rotationY.Mul(editor_camera->frustum.front).Normalized();
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			editor_camera->Position = vec(5.0f, 5.0f, 5.0f);
			editor_camera->Reference = { 0,0,0 };
		}



	}
	return UPDATE_CONTINUE;
}

LineSegment ModuleCamera3D::getMouseClickRay()
{
	LineSegment ret;
	//ret = editor_camera->frustum.UnProjectLineSegment();
	return ret;
}

void ModuleCamera3D::CameraAdaptation(vec new_pos, vec new_ref)
{
	editor_camera->Position = new_pos;
	editor_camera->Position += CAMERA_OFFSET*new_pos;
	editor_camera->Reference = new_ref;

	editor_camera->LookAt(editor_camera->Reference);

}


// -----------------------------------------------------------------


