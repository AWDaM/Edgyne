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

	CalculateViewMatrix();
	X = vec(1.0f, 0.0f, 0.0f);
	Y = vec(0.0f, 1.0f, 0.0f);
	Z = vec(0.0f, 0.0f, 1.0f);
	
	
	
	Position = vec(5.0f, 5.0f, 5.0f);
	Reference = vec(0.0f, 0.0f, 0.0f);	
	LookAt(Reference);
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

	return true;
}

void ModuleCamera3D::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
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
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= editor_camera->Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += editor_camera->Z * speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= editor_camera->X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += editor_camera->X * speed;
			
		}

		if (App->input->GetMouseZ() > 0)
		{
			newPos += editor_camera->Z * speed * CAMERA_SPEED_MULTIPLIER;
		}
		else if (App->input->GetMouseZ() < 0)
		{
			newPos -= editor_camera->Z * speed * CAMERA_SPEED_MULTIPLIER;
		}

		editor_camera->Move(newPos);
		editor_camera->frustum.SetPos(editor_camera->frustum.Pos() + newPos);

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			editor_camera->Position -= editor_camera->Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				editor_camera->X = *(float3*)&(float4x4::RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX)*float4(editor_camera->X, 1.0f));
				editor_camera->Y = *(float3*)&(float4x4::RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX)*float4(editor_camera->Y, 1.0f));
				editor_camera->Z = *(float3*)&(float4x4::RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX)*float4(editor_camera->Z, 1.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				editor_camera->Y = *(float3*)&(float4x4::RotateAxisAngle(editor_camera->X, DeltaY)*float4(editor_camera->Y, 1.0f));
				editor_camera->Y = *(float3*)&(float4x4::RotateAxisAngle(editor_camera->X, DeltaY)*float4(editor_camera->Z, 1.0f));

				if (editor_camera->Y.y < 0.0f)
				{
					editor_camera->Z = vec(0.0f, editor_camera->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					editor_camera->Y = editor_camera->Z.Cross(editor_camera->X);
				}
			}

			editor_camera->Position = editor_camera->Reference + editor_camera->Z * editor_camera->Position.Length();
			editor_camera->Look(editor_camera->Position, { 0,0,0 }, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			editor_camera->X = vec(1.0f, 0.0f, 0.0f);
			editor_camera->Y = vec(0.0f, 1.0f, 0.0f);
			editor_camera->Z = vec(0.0f, 0.0f, 1.0f);

			editor_camera->Position = vec(5.0f, 5.0f, 5.0f);
			editor_camera->Reference = { 0,0,0 };
			editor_camera->Look(editor_camera->Position, editor_camera->Reference);
		}

		// Recalculate matrix -------------


	editor_camera->CalculateViewMatrix();
	}
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec &Position, const vec &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec &Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

LineSegment ModuleCamera3D::getMouseClickRay()
{
	LineSegment ret;
	//ret = editor_camera->frustum.UnProjectLineSegment();
	return ret;
}

void ModuleCamera3D::CameraAdaptation(vec new_pos, vec new_ref)
{
	Position = new_pos;
	Position += CAMERA_OFFSET*new_pos;
	Reference = new_ref;

	Look(Position, Reference);
	CalculateViewMatrix();
}


// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix[0][0];
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}