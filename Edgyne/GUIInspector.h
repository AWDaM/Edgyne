#pragma once
#include "GUIElements.h"
#include "MathGeoLib\Math\float3.h"

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
private:
public:
private:

};

