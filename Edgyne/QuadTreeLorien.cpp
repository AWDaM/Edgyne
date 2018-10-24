#include "QuadTreeLorien.h"
#include "GameObject.h"



QuadTreeLorien::QuadTreeLorien(AABB root)
{
	root_node = new QuadTreeNodeLorien(root);
}

QuadTreeLorien::~QuadTreeLorien()
{
}

void QuadTreeLorien::Clear()
{
}

void QuadTreeLorien::Insert(GameObject * game_object)
{
	if (game_object->aligned_bounding_box.CenterPoint().Distance(root_node->aabb) == 0)
	{
		if (root_node->leaves.size() >= bucket_size)
		{
			root_node->SubdivideAndFit(game_object);
		}
		else
			root_node->leaves.push_back(game_object);
	}
}

template<typename TYPE>
 void QuadTreeLorien::CollectIntersections(std::vector<GameObject*>& buffer, const TYPE & primitive)
{
	 if (primitive.Intersects(root_node->aabb))
	 {
		 std::vector<GameObject*>::iterator item = root_node->leaves.begin();

		 while (item != root_node->leaves.end())
		 {
			 if (primitive.Intersects((*item)->aligned_bounding_box))
				 buffer.push_back(*item);
			 item++;
		 }

		 for (int i = 0; i < 4; i++)
			 if (root_node->children[i] != nullptr)
				 root_node->children[i]->CollectIntersections(buffer, primitive);
	 }
}

QuadTreeNodeLorien::QuadTreeNodeLorien(AABB aabb, QuadTreeLorien* tree) : aabb(aabb), tree(tree)
{

}

void QuadTreeNodeLorien::SubdivideAndFit(GameObject* game_object)
{
	AABB tmp;

	tmp.maxPoint = aabb.maxPoint - vec(aabb.HalfSize.x, 0, 0);
	tmp.minPoint = aabb.minPoint + vec(0, 0, aabb.HalfSize.z);
	children[0] = new QuadTreeNodeLorien(tmp,tree);

	tmp.maxPoint = aabb.CenterPoint() + vec(0, aabb.HalfSize().y, 0);
	tmp.minPoint = aabb.minPoint;
	children[1] = new QuadTreeNodeLorien(tmp,tree);

	tmp.maxPoint = aabb.maxPoint;
	tmp.minPoint = aabb.CenterPoint() - vec(0, aabb.HalfSize().y, 0);
	children[2] = new QuadTreeNodeLorien(tmp,tree);

	tmp.maxPoint = aabb.maxPoint - vec(0, 0, aabb.HalfSize().z);
	tmp.minPoint = aabb.minPoint + vec(aabb.HalfSize().x, 0, 0);
	children[3] = new QuadTreeNodeLorien(tmp,tree);

	for (int i = 0; i < leaves.size(); i++)
	{
		FitGameObject(leaves[i]);
	}
	FitGameObject(game_object);
}

void QuadTreeNodeLorien::FitGameObject(GameObject* game_object)
{
	for (int i = 0; i < 4; i++)
	{
		if (children[i]->aabb.Contains(game_object->aligned_bounding_box.CenterPoint()))
		{
			if (children[i]->leaves.size() >= tree->bucket_size)
			{
				SubdivideAndFit(game_object);
			}
			else
			{
				leaves.push_back(game_object);
			}
		}
	}
}

template<typename TYPE>
void QuadTreeNodeLorien::CollectIntersections(std::vector<GameObject*>& buffer, const TYPE & primitive)
{
	if (primitive.Intersects(aabb))
	{
		std::vector<GameObject*>::iterator item = leaves.begin();

		while (item != leaves.end())
		{
			if (primitive.Intersects((*item)->aligned_bounding_box))
				buffer.push_back(*item);
			item++;
		}

		for (int i = 0; i < 4; i++)
			if (children[i] != nullptr)
				children[i]->CollectIntersections(buffer, primitive);
	}
}

