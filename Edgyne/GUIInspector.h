#ifndef __GUI_INSPECTOR_H__
#define __GUI_INSPECTOR_H__


#include "GUIElements.h"
#include "MathGeoLib\Math\float3.h"

class GameObject;

struct transformation
{
	vec position;
	vec rotation;
	vec scale;
};
class GUIInspector : public GUIElements
{
public:
	GUIInspector(bool active);
	~GUIInspector();

	void Draw();
	bool Load(rapidjson::Value& Node);
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);

	void AddComponents();
private:
public:
	GameObject* highlighted_game_object = nullptr;
private:

};

#endif // !__GUI_INSPECTOR_H__