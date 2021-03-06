#ifndef __QUAD_TREE_CHECHU_H__
#define __QUAD_TREE_CHECHU_H__


#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"
#include <vector>
#include "Globals.h"

struct quadTreeNode
{
public:
	quadTreeNode(AABB bb, uint myDepth, uint bucketSize = 1) : boundingBox(bb), bucketSize(bucketSize), myDepth(myDepth)
	{ 
		for (int i = 0; i < 4; i++)
			children[i] = nullptr;
	};
	~quadTreeNode()
	{
		go = nullptr;

		myObjects.clear();
	}
public:
	GameObject * go = nullptr;
	AABB boundingBox;
	quadTreeNode* children[4];
	uint bucketSize;
	uint maximumDepth = 10;
	uint myDepth = 1;
	std::vector<GameObject*> myObjects;


	bool isDivided = false;

	void SubdivideNode(AABB bb, uint currentDepth, uint bucketSize = 1)
	{
		AABB childrenBB;
		
		//								from min x and middle z																to middle x and max z
		childrenBB = AABB({bb.minPoint.x, bb.minPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 },								{ (bb.maxPoint.x + bb.minPoint.x) / 2, bb.maxPoint.y, bb.maxPoint.z });
		children[0] = new quadTreeNode(childrenBB, currentDepth, bucketSize);

		//								from middle x and middle z															to max x and max z
		childrenBB = AABB({(bb.maxPoint.x + bb.minPoint.x) / 2, bb.minPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 },		bb.maxPoint);
		children[1] = new quadTreeNode(childrenBB, currentDepth, bucketSize);

		//								from min x and min z																to middle x and middle z
		childrenBB = AABB(bb.minPoint,																						{ (bb.maxPoint.x + bb.minPoint.x) / 2, bb.maxPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 });
		children[2] = new quadTreeNode(childrenBB, currentDepth, bucketSize);

		//								from middle x and min z																to max x and middle y
		childrenBB = AABB({(bb.maxPoint.x + bb.minPoint.x) / 2, bb.minPoint.y, bb.minPoint.z },								{ bb.maxPoint.x, bb.minPoint.y, (bb.maxPoint.z + bb.minPoint.z) / 2 });
		children[3] = new quadTreeNode(childrenBB, currentDepth, bucketSize);

		AddPreviousNodesToTheirCorrespondingChild();
	}
	
	void AddPreviousNodesToTheirCorrespondingChild()
	{
		for (int i = 0; i < myObjects.size(); i++)
		{
			CheckChildrenIntersections(myObjects[i]);
		}
	}

	bool CheckChildrenIntersections(GameObject* obj)
	{
		bool ret = false;
		if (children[0]->boundingBox.Intersects(obj->aligned_bounding_box))
			ret = children[0]->InsertPrimitive(obj);
		if (children[1]->boundingBox.Intersects(obj->aligned_bounding_box))
			ret = children[1]->InsertPrimitive(obj);
		if (children[2]->boundingBox.Intersects(obj->aligned_bounding_box))
			ret = children[2]->InsertPrimitive(obj);
		if (children[3]->boundingBox.Intersects(obj->aligned_bounding_box))
			ret = children[3]->InsertPrimitive(obj);

		return ret;
	}

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& buffer, const TYPE & primitive)
	{
		if (primitive.Intersects(boundingBox))
		{
			if (myObjects.size() > 0)
			{
				std::vector<GameObject*>::iterator item = myObjects.begin();

				while (item != myObjects.end())
				{
					if (!(*item)->added_to_quadtree_buffer)
					{
						buffer.push_back(*item);
						(*item)->added_to_quadtree_buffer = true;
					}
					item++;
				}
			}
			for (int i = 0; i < 4; i++)
				if (children[i] != nullptr)
					children[i]->CollectIntersections(buffer, primitive);
		}
	}

	bool InsertPrimitive(GameObject* obj)
	{
		bool ret = false;
		
		//compare if gameobject is inside the boundingBox
		if (boundingBox.Intersects(obj->aligned_bounding_box))
		{
			if (myObjects.size() + 1 > bucketSize && isDivided && myDepth < maximumDepth)
			{
				ret = CheckChildrenIntersections(obj);
			}
			else if (myObjects.size() + 1 > bucketSize && myDepth < maximumDepth)
			{
				SubdivideNode(boundingBox, myDepth + 1, bucketSize);
				isDivided = true;
				ret = CheckChildrenIntersections(obj);
				myObjects.clear();
			}
			else
			{
				for (std::vector<GameObject*>::iterator item = myObjects.begin(); item != myObjects.end(); item++)
					if ((*item) == obj)
						return true;

				myObjects.push_back(obj);
				ret = true;
			}
		}
		return ret;
	}

	void Clear()
	{
		if (isDivided)
		{
			for (int i = 0; i < 4; i++)
			{
				children[i]->Clear();
			}
		}
		delete this;
	}
};
class EdgyQuadTree
{
public:
	EdgyQuadTree(uint bucketSize = 1) : bucketSize(bucketSize) {};
	~EdgyQuadTree() { RELEASE(root_node) };

	void Create(AABB limits)
	{
		root_node = new quadTreeNode(limits, bucketSize);
	};

	void Clear()
	{
		if (root_node->isDivided)
		{
			for (int i = 0; i < 4; i++)
			{
				root_node->children[i]->Clear();
			}
		}
		root_node->isDivided = false;
	};

	bool Insert(GameObject* obj)
	{
		return root_node->InsertPrimitive(obj);
	};
	bool Remove(GameObject*);

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& buffer, const TYPE & primitive)
	{
		root_node->CollectIntersections(buffer, primitive);
	}


	

public:
	uint bucketSize;
	quadTreeNode* root_node = nullptr;
};

#endif // !__QUAD_TREE_CHECHU_H__