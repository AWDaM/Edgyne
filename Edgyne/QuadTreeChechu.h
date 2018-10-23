#pragma once
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"
#include <vector>
#include "Globals.h"

class QuadTreeChechu
{
public:
	QuadTreeChechu(uint bucketSize) : bucketSize(bucketSize) {};
	~QuadTreeChechu() {};

	bool Create(AABB limits)
	{
		quadtreeBox = limits;

	};
	void Clear();
	bool Insert(GameObject*);
	bool Remove(GameObject*);
	bool Intersect(std::vector<GameObject*>&);

public:
	AABB quadtreeBox;
	uint bucketSize;
};