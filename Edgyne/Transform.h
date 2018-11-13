#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__


#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"

class Transform :
	public Component
{
public:
	Transform();
	Transform(GameObject* game_object);
	~Transform();

	float4x4 LocalTransformationMatrix();
	void TransformChanged();
	void OnEditor();

	rapidjson::Value SaveToScene(rapidjson::Document::AllocatorType& allocator);
private:
public:
	vec position;
	Quat rotation;
	vec rotation_euler;
	vec previous_rotation_euler;
	vec scale;
private:

};

#endif // !__TRANSFORM_H__