#ifndef __GUI_ASSETS_H__
#define __GUI_ASSETS_H__


#include "GUIElements.h"

class GUIAssets :
	public GUIElements
{
public:
	GUIAssets(bool active);
	~GUIAssets();
	void Draw();
	bool Load(rapidjson::Value& Node);
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);

	std::string currentPath = "Assets\\";
};
#endif // !__GUI_ASSETS_H__


