#include "Application.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "ModuleDebug.h"

#include "SDL\include\SDL_opengl.h"

#include "ModuleLoader.h"

#include "QuadTreeChechu.h"

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

	QuadTreeChechu quadTree(1);
	quadTree.Create(test);

	AABB* firstCube = new AABB({ 1,0,1 }, { 2,1,2 });
	AABB* secondCube = new AABB({7,0,7}, {8,1,8});

	/*quadTree.Insert(firstCube);
	quadTree.Insert(secondCube);*/
	root = NewGameObject("root");

	return true;
}

bool ModuleLevel::Start()
{
	//App->loader->ReceivedFile("Assets\\BakerHouse\\BakerHouse.FBX");
	GameObject* camera = NewGameObject("Camera");
	camera->AddComponent(CAMERA);
	camera->tag = MAIN_CAMERA;
		
		return true;
}

GameObject * ModuleLevel::NewGameObject(std::string name, bool with_transform)
{
	GameObject* ret = new GameObject(name);
	if (with_transform)
	{
		ret->AddComponent(TRANSFORM);
	}
	game_objects.push_back(ret);
	return ret;
}

void ModuleLevel::Draw()
{
	std::vector<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		(*item)->Draw();
		if ((*item)->tag == MAIN_CAMERA)
		{
			App->debug->Draw_Camera((Camera*)(*item)->components[0]);
		}
		item++;
	}

}
