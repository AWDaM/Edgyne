#ifndef __GUI_SCENE__
#define __GUI_SCENE__


#include "GUIElements.h"
class GUIScene :
	public GUIElements
{
public:
	GUIScene(bool active);
	~GUIScene();

	void Draw();


	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
	bool Load(rapidjson::Value& Node);

	bool IsMouseHovering();

	bool hovered = false;

};

#endif // !__GUI_SCENE__