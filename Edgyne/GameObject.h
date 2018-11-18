#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"

#include "rapidjson/document.h"// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"
#include "rapidjson\allocators.h"
#include "MathGeoLib/MathGeoLib.h"

#include <vector>
#include <string>
#include <list>

enum ComponentType
{
	NO_TYPE,
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA
};

enum Tag {
	NO_TAG,
	MAIN_CAMERA
};

enum Layer {
	NO_LAYER
};

class Component;
class Transform;

class GameObject
{
public:
	GameObject(GameObject* parent, std::string _name = "new GameObject");
	~GameObject();

	bool Update();
	bool Draw();

	bool InitComponents();
	bool UpdateComponents();
	bool CleanUpComopnents();

	void SaveToScene(rapidjson::Value& obj, rapidjson::Document::AllocatorType& al);

	void OnHierarchy(int id);

	void OnInspector();

	void RecursiveSetChildsActive(bool active);
	void RecursiveSetChildsStatic(bool _static);
	void RecursiveSetChildsTransformChanged(bool transform_changed);

	void SetLocalBoundingVolume();

	Component* AddComponent(ComponentType type);
	GameObject* AddGameObject(std::string name, bool with_transform = true);
	void RecursiveTransformChanged(const float4x4& parent);
	void RemoveSelfFromParent();
	void RecursiveDeleteGameObject();
	void RecursiveResetAddedToQuadTree();
	Component* GetComponent(ComponentType type);

	void LinkMeshWithMaterial();

private:
public:
	std::string name;
	bool Static = true;
	bool active = false;
	GameObject* parent = nullptr;

	Tag tag = NO_TAG;
	AABB aligned_bounding_box;
	Sphere bounding_sphere;
	OBB bounding_box;
	std::vector<Component*> components;
	Transform* transform = nullptr;
	float4x4 global_transform_matrix;
	bool transform_changed = false;
	uint UID = 0;
	uint parentUID = 0;
	std::vector<uint> childrenUID;
	bool to_remove = false;
	std::list<GameObject*> childs;
	bool added_to_quadtree_buffer = false;
private:


};

#endif // !__GAME_OBJECT_H__