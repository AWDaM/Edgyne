#pragma once
#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"

class Transform :
	public Component
{
public:
	Transform();
	~Transform();


private:
public:
	vec position;
	Quat rotation;
	vec scale;
private:

};

