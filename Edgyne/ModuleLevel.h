#ifndef __MODULE_LEVEL_H__
#define __MODULE_LEVEL_H__


#include "Module.h"
#include "QuadTreeChechu.h"

#include <vector>

class GameObject;
class Camera;

class ModuleLevel :
	public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();
	bool Init(rapidjson::Value& node);
	bool Start();

	update_status Update(float dt);
	GameObject* NewGameObject(std::string name, bool with_transform = true);
	
	void Draw();
private:
public:
	QuadTreeChechu quadTree;
	std::vector<GameObject*> game_objects;
	GameObject* root = nullptr;
	Camera* game_camera = nullptr;
	GameObject* selected_game_object = nullptr;
};

#endif // !__MODULE_LEVEL_H__