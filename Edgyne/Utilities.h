#pragma once
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

vec3 GetVec3From_btVec3(btVector3 vector)
{
	vec3 ret;
	
	ret.x = vector.x();
	ret.y = vector.y();
	ret.z = vector.z();

	return ret;
}

//btVector3 GetbtVec3From_vec3(vec3 vector)
//{
//	//btVector3 ret;
//	//ret.x() = vector.x;
//	//ret.y() = vector.y;
//	//ret.z()= vector.z;
//
//	return ret;
//}