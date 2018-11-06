#include "Application.h"
#include "ModuleLevel.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Camera.h"
#include "ModuleDebug.h"

#include "SDL\include\SDL_opengl.h"

#include "ModuleLoader.h"



ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Level";
}


ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Init(rapidjson::Value& node)
{
	AABB test({ -1,-1,-1 }, { 10, 2, 10 });

	quadTree.Create(test);

	AABB firstCube({ 1,0,1 }, { 2,1,2 });
	AABB secondCube({7,0,7}, {8,1,8});
	AABB thirdCube({2,0,1}, {4,1,3});
	AABB fourthCube({5,0,5}, {6,1,8});
	AABB fifthCube({2,0,1}, {4,1,4});

	quadTree.Insert(firstCube);
	quadTree.Insert(secondCube);
	quadTree.Insert(thirdCube);
	quadTree.Insert(fourthCube);
	quadTree.Insert(fifthCube);

	root = new GameObject(nullptr, "root");
	root->transform = (Transform*)root->AddComponent(TRANSFORM);

	return true;
}

bool ModuleLevel::Start()
{
	//App->loader->ReceivedFile("Assets\\BakerHouse\\BakerHouse.FBX");
	GameObject* camera = NewGameObject("Camera");
	game_camera = (Camera*)camera->AddComponent(CAMERA);
	camera->tag = MAIN_CAMERA;
		
		return true;
}


update_status ModuleLevel::Update(float dt)
{
	std::vector<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		if ((*item)->active)
		{
			(*item)->Update();
		}
		item++;
	}
	return UPDATE_CONTINUE;
}

GameObject * ModuleLevel::NewGameObject(std::string name, bool with_transform)
{
	GameObject* ret = root->AddGameObject(name, with_transform);

	game_objects.push_back(ret);
	return ret;
}

void ModuleLevel::Draw()
{
	root->CalcGlobalTransform(root->global_transform_matrix);
	std::vector<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		if ((*item)->active)
		{
			if (game_camera->frustum.ContainsAABBCustom((*item)->aligned_bounding_box)&& App->renderer3D->camera_culling)
			{
				(*item)->Draw();
			}
	
			else if (!App->renderer3D->camera_culling)
			{
				(*item)->Draw();
			}

			if (selected_game_object)
			{
				App->debug->Draw_AABB(selected_game_object->aligned_bounding_box);
			}

		App->debug->Draw_Camera(game_camera);

		}
		item++;
	}

}
