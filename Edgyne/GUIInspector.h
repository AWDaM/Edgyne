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
	GUIInspector();
	~GUIInspector();

	void Draw();
private:
public:
private:

};

