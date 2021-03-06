#ifndef __MODULE_LEVEL_H__
#define __MODULE_LEVEL_H__


#include "Module.h"


#include <vector>
#include <map>

class GameObject;
class Camera;
class EdgyQuadTree;
class Mesh;

class ModuleLevel :
	public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();
	bool Init(rapidjson::Value& node);
	bool Start();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	GameObject* NewGameObject(std::string name, bool with_transform = true);
	std::vector<GameObject*> GetNonStaticObjects();
	void DeleteLevel();
	
	void Draw();

	void GenerateDistanceToCamMap(std::map<float, Mesh*>& buffer, std::vector<GameObject*> objects_to_draw,math::Frustum camera);

	GameObject* MousePicking(int posX, int posY);
private:
public:
	EdgyQuadTree* quad_tree;
	std::list<GameObject*> game_objects;
	GameObject* root = nullptr;
	Camera* game_camera = nullptr;
	GameObject* selected_game_object = nullptr;
	bool generate_quadtree = false;
	std::string current_scene_name;
	math::LineSegment ray;
};

#endif // !__MODULE_LEVEL_H__