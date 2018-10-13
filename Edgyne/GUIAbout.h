#pragma once
#include "GUIElements.h"
class GUIAbout :
	public GUIElements
{
public:
	GUIAbout();
	~GUIAbout();

	void Draw();
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
};

