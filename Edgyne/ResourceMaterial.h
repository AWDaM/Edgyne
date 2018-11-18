#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Resource.h"


class ResourceMaterial :
	public Resource
{
public:
	ResourceMaterial(uint uid, rapidjson::Value::ConstMemberIterator comp);
	~ResourceMaterial();

	void ResourceMaterial::LoadComponent(rapidjson::Value::ConstMemberIterator comp);

	vec color;
	float2 img_size;
	uint id_texture = 0;
	std::string fileName;
};

#endif
