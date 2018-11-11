#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
#include "Globals.h"

class GameObject;

class Material :
	public Component
{
public:
	Material();
	Material(GameObject* game_object);
	~Material();

	rapidjson::Value SaveToScene(rapidjson::Document::AllocatorType& allocator);
	
	bool ComponentDraw();
	void MaterialBind();
	void MaterialUnbind();
	void OnEditor();

private:
public:
	vec color;
	float2 img_size;
	uint id_texture = 0;
	std::string fileName;
private:

};

#endif // !__MATERIAL_H__