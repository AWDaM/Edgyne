#ifndef __GUI_HIERARCHY_H__
#define __GUI_HIERARCHY_H__



#include "GUIElements.h"
class GUIHierarchy : public GUIElements
{
public:
	GUIHierarchy(bool active);
	~GUIHierarchy();

	void Draw();
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
	bool Load(rapidjson::Value& Node);

};

#endif // !__GUI_HIERARCHY_H__