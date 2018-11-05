#ifndef __QUAD_TREE_LORIEN_H__
#define __QUAD_TREE_LORIEN_H__


#include "MathGeoLib\MathGeoLib.h"

class GameObject;
class QuadTreeNodeLorien;

class QuadTreeLorien
{
public:
	QuadTreeLorien(AABB root);
	~QuadTreeLorien();

	QuadTreeNodeLorien* root_node;


	void Clear();
	void Insert(GameObject* game_object);

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& buffer, const TYPE & primitive);
	int bucket_size;

public:
	
};

class QuadTreeNodeLorien
{
public:
	QuadTreeNodeLorien(AABB aabb, QuadTreeLorien* tree);

	AABB aabb;
	QuadTreeNodeLorien* children[4];
	void SubdivideAndFit(GameObject* game_object);

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& buffer, const TYPE & primitive);

	QuadTreeLorien* tree;

	void FitGameObject(GameObject* game_object);
	std::vector<GameObject*> leaves;
};

#endif // !__QUAD_TREE_LORIEN_H__


