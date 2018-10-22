#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
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
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
			
		}

		if (App->input->GetMouseZ() > 0)
		{
			newPos += Z * speed * CAMERA_SPEED_MULTIPLIER;
		}
		else if (App->input->GetMouseZ() < 0)
		{
			newPos -= Z * speed * CAMERA_SPEED_MULTIPLIER;
		}

		Move(newPos);

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = *(float3*)&(float4x4::RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX)*float4(X, 1.0f));
				Y = *(float3*)&(float4x4::RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX)*float4(Y, 1.0f));
				Z = *(float3*)&(float4x4::RotateAxisAngle(vec(0.0f, 1.0f, 0.0f), DeltaX)*float4(Z, 1.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = *(float3*)&(float4x4::RotateAxisAngle(X, DeltaY)*float4(Y, 1.0f));
				Y = *(float3*)&(float4x4::RotateAxisAngle(X, DeltaY)*float4(Z, 1.0f));

				if (Y.y < 0.0f)
				{
					Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = Z.Cross(X);
				}
			}

			Position = Reference + Z * Position.Length();
			Look(Position, { 0,0,0 }, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			X = vec(1.0f, 0.0f, 0.0f);
			Y = vec(0.0f, 1.0f, 0.0f);
			Z = vec(0.0f, 0.0f, 1.0f);

			Position = vec(5.0f, 5.0f, 5.0f);
			Reference = { 0,0,0 };
			Look(Position, Reference);
		}

		// Recalculate matrix -------------

		CalculateViewMatrix();
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