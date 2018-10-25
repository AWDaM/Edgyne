#pragma once
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"
#include <vector>
#include "Globals.h"

struct quadTreeNode
{
public:
	quadTreeNode(AABB bb, uint bucketSize = 1) : boundingBox(bb), bucketSize(bucketSize) 
	{ 
		for (int i = 0; i < 4; i++)
			children[i] = nullptr;
	};
public:
	AABB boundingBox;
	quadTreeNode* children[4];
	uint bucketSize;
	std::vector<AABB> myObjects;

	bool isDivided = false;


	void SubdivideNode(AABB bb, uint bucketSize = 1)
	{
		AABB childrenBB;
		
		//								from min x and middle z																to middle x and max z
		childrenBB = AABB({bb.minPoint.x, bb.minPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 },								{ (bb.maxPoint.x + bb.minPoint.x) / 2, bb.maxPoint.y, bb.maxPoint.z });
		children[0] = new quadTreeNode(childrenBB, bucketSize);

		//								from middle x and middle z															to max x and max z
		childrenBB = AABB({ (bb.maxPoint.x + bb.minPoint.x) / 2, bb.minPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 },		bb.maxPoint);
		children[1] = new quadTreeNode(childrenBB, bucketSize);

		//								from min x and min z																to middle x and middle z
		childrenBB = AABB(bb.minPoint,																						{ (bb.maxPoint.x + bb.minPoint.x) / 2, bb.maxPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 });
		children[2] = new quadTreeNode(childrenBB, bucketSize);

		//								from middle x and min z																to max x and middle y
		childrenBB = AABB({ (bb.maxPoint.x + bb.minPoint.x) / 2, bb.minPoint.y, bb.minPoint.z },							{ bb.maxPoint.x, bb.minPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 });
		children[3] = new quadTreeNode(childrenBB, bucketSize);

		AddPreviousNodesToTheirCorrespondingChild();
	}
	
	void AddPreviousNodesToTheirCorrespondingChild()
	{
		for (int i = 0; i < myObjects.size(); i++)
		{
			CheckChildrenIntersections(myObjects[i])->InsertPrimitive(myObjects[i]);
		}
	}

	quadTreeNode* CheckChildrenIntersections(AABB obj)
	{
		quadTreeNode* ret;
		if (children[0]->boundingBox.Intersects(obj))
			return children[0];
		else if (children[1]->boundingBox.Intersects(obj))
			return children[1];
		else if (children[2]->boundingBox.Intersects(obj))
			return children[2];
		else if (children[3]->boundingBox.Intersects(obj))
			return children[3];
	}

	bool InsertPrimitive(AABB obj)
	{
		bool ret = false;
		
		//compare if gameobject is inside the boundingBox
		if (boundingBox.Intersects(obj))
		{
			if (myObjects.size() + 1 > bucketSize && isDivided)
			{
				ret = CheckChildrenIntersections(obj)->InsertPrimitive(obj);
			}
			else if (myObjects.size() + 1 > bucketSize)
			{
				SubdivideNode(boundingBox, bucketSize);
				isDivided = true;
				ret = CheckChildrenIntersections(obj)->InsertPrimitive(obj);
				myObjects.clear();
			}
			else
			{
				myObjects.push_back(obj);
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
	bool Insert(AABB obj)
	{
		return rootNode->InsertPrimitive(obj);
	};
	bool Remove(GameObject*);
	bool Intersect(std::vector<GameObject*>&);

public:
	uint bucketSize;
	quadTreeNode* rootNode = nullptr;
};