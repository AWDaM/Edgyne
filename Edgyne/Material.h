#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
class GameObject;

class Material :
	public Component
{
public:
	Material();
	Material(GameObject* game_object);
	~Material();
};

#endif // !__MATERIAL_H__