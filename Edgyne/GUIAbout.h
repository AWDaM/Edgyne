#ifndef __GUI_ABOUT_H__
#define __GUI_ABOUT_H__


#include "GUIElements.h"
class GUIAbout :
	public GUIElements
{
public:
	GUIAbout(bool active);
	~GUIAbout();

	void Draw();
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
	bool Load(rapidjson::Value& Node);
};

#endif // !__GUI_ABOUT_H__