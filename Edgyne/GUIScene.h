#pragma once
#include "GUIElements.h"
class GUIScene :
	public GUIElements
{
public:
	GUIScene();
	~GUIScene();

	void Draw();
	bool IsMouseHovering();

	bool hovered = false;
};

