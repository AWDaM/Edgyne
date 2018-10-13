#pragma once
#include "GUIElements.h"
class GUIConfiguration :
	public GUIElements
{
public:
	GUIConfiguration();
	~GUIConfiguration();
	void Draw();
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
};

