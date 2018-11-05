#ifndef __GUI_CONFIGURATION__
#define __GUI_CONFIGURATION__


#include "GUIElements.h"
class GUIConfiguration :
	public GUIElements
{
public:
	GUIConfiguration(bool active);
	~GUIConfiguration();
	void Draw();
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
	bool Load(rapidjson::Value& Node);
};

#endif // !__GUI_CONFIGURATION__