#ifndef __MESH_H__
#define __MESH_H__



#include "Component.h"
#include "Globals.h"

class Material;
class MeshResource;

class Mesh : public Component
{
public:
	Mesh();
	Mesh(GameObject* game_object);
	~Mesh();

	rapidjson::Value SaveToScene(rapidjson::Document::AllocatorType& allocator);
	void LoadComponent(rapidjson::Value::ConstMemberIterator comp);

	void OnEditor();
	bool ComponentUpdate();
	bool ComponentDraw();
	void SetBoundingVolume();
	bool IsOpaque();
private:
public:
	std::string resource_mesh;
	std::string material_name;

	Material* material = nullptr;

private:
	
};

#endif // !__MESH_H__