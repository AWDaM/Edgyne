#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
#include "Globals.h"

class GameObject;
class Mesh;
class ResourceMaterial;

class Material :
	public Component
{
public:
	Material();
	Material(GameObject* game_object);
	~Material();

	rapidjson::Value SaveToScene(rapidjson::Document::AllocatorType& allocator);
	void LoadComponent(rapidjson::Value::ConstMemberIterator comp);
	void Material::LoadAsMeshComponent(rapidjson::Value::ConstMemberIterator comp);


	bool ComponentDraw();
	void OnEditor();

private:
public:
	uint resource_uid = 0;
private:

};

#endif // !__MATERIAL_H__