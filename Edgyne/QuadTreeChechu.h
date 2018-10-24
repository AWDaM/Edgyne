#pragma once
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"
#include <vector>
#include "Globals.h"
#include <stdio.h>


struct quadTreeNode
{
public:
	quadTreeNode(AABB bb, uint bucketSize = 1) : boundingBox(bb), bucketSize(bucketSize) 
	{ 
		myObjects = new AABB[bucketSize]; 
		for (int i = 0; i < 4; i++)
		{
			children[i] = nullptr;
		}
	};

public:
	quadTreeNode* children[4];
	AABB boundingBox;
	uint bucketSize;
	AABB* myObjects;

	uint numOfObjects = 0;


	void SubdivideNode(AABB bb, uint bucketSize = 1)
	{
		AABB childrenBB;
		
		//								from min x and middle y													to middle x and max y
		childrenBB = AABB({ (bb.maxPoint.x + bb.minPoint.x) / 2, 0, bb.minPoint.z },							{ bb.maxPoint.x, 0, (bb.maxPoint.z + bb.minPoint.z) / 2 });	
		children[0] = new quadTreeNode(childrenBB, bucketSize);

		//								from middle x and middle y												to max x and max y
		childrenBB = AABB({ (bb.maxPoint.x + bb.minPoint.x) / 2, 0, (bb.maxPoint.z + bb.minPoint.z) / 2 },		bb.maxPoint);
		children[1] = new quadTreeNode(childrenBB, bucketSize);

		//								from min x and min y													to middle x and middle y
		childrenBB = AABB(bb.minPoint,																			{ (bb.maxPoint.x + bb.minPoint.x) / 2, 0, (bb.maxPoint.z + bb.minPoint.z) / 2 });
		children[2] = new quadTreeNode(childrenBB, bucketSize);

		//								from middle x and min y													to max x and middle y
		childrenBB = AABB({ (bb.maxPoint.x + bb.minPoint.x) / 2, 0, bb.minPoint.z },							{ bb.maxPoint.x, 0, (bb.maxPoint.z + bb.minPoint.z) / 2 });
		children[3] = new quadTreeNode(childrenBB, bucketSize);

		AddPreviousNodesToTheirCorrespondingChild();
	}
	
	void AddPreviousNodesToTheirCorrespondingChild()
	{
		for (int i = 0; i < numOfObjects; i++)
		{
			CheckChildrenIntersections(&myObjects[i])->InsertPrimitive(&myObjects[i]);
		}
	}

	quadTreeNode* CheckChildrenIntersections(AABB* obj)
	{
		quadTreeNode* ret;
		if (children[0]->boundingBox.Intersects(*obj))
			ret = children[0];
		else if (children[1]->boundingBox.Intersects(*obj))
			ret = children[1];
		else if (children[2]->boundingBox.Intersects(*obj))
			ret = children[2];
		else if (children[3]->boundingBox.Intersects(*obj))
			ret = children[3];

		return ret;
	}

	bool InsertPrimitive(AABB* obj)
	{
		bool ret = false;
		numOfObjects += 1;
		//compare if gameobject is inside the boundingBox
		if (boundingBox.Intersects(*obj))
		{
			if (numOfObjects > bucketSize)
			{
				quadTreeNode* intersectingChild = nullptr;
				numOfObjects -= 1;
				SubdivideNode(boundingBox, bucketSize);
				ret = CheckChildrenIntersections(obj)->InsertPrimitive(obj);
			}
			else
			{
				myObjects[numOfObjects - 1] = *obj;
				ret = true;
			}
		}
		return ret;
	}
	// call the main method with the perimeter of the primitive
	// check if its inside the node
	// if it is, check if it still meets the bucket size
	// if it doesn't, recall the function for each of the children and all the objects
	// repeat untill all nodes have <= sucket size
};
class QuadTreeChechu
{
public:
	QuadTreeChechu(uint bucketSize) : bucketSize(bucketSize) {};
	~QuadTreeChechu() {};

	void Create(AABB limits)
	{
		rootNode = new quadTreeNode(limits, bucketSize);
	};
	void Clear();
	bool Insert(AABB* obj)
	{
		return rootNode->InsertPrimitive(obj);
	};
	bool Remove(GameObject*);
	bool Intersect(std::vector<GameObject*>&);

public:
	uint bucketSize;
	quadTreeNode* rootNode = nullptr;
};