#pragma once
#include "GUIElements.h"
class GUIScene :
	public GUIElements
{
public:
	GUIScene();
	~GUIScene();

	void Draw();

	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
};

