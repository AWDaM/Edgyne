#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Resource.h"


class ResourceMaterial :
	public Resource
{
public:
	ResourceMaterial(std::string& file);
	ResourceMaterial();
	~ResourceMaterial();

	void ResourceMaterial::LoadMaterial(std::string& file);

	vec color;
	float2 img_size;
	uint id_texture = 0;
};

#endif
