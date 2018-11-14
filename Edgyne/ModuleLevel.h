#ifndef __MODULE_LEVEL_H__
#define __MODULE_LEVEL_H__


#include "Module.h"


#include <vector>

class GameObject;
class Camera;
class EdgyQuadTree;

class ModuleLevel :
	public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();
	bool Init(rapidjson::Value& node);
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	GameObject* NewGameObject(std::string name, bool with_transform = true);
	void DeleteLevel();
	
	void Draw();
private:
public:
	EdgyQuadTree* quad_tree;
	std::list<GameObject*> game_objects;
	GameObject* root = nullptr;
	Camera* game_camera = nullptr;
	GameObject* selected_game_object = nullptr;
	bool generate_quadtree = false;
};

#endif // !__MODULE_LEVEL_H__