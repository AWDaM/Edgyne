#pragma once
#include "Globals.h"
#include <string>
#include "Imgui/imgui.h"
#include "rapidjson/document.h"// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"

class GUIElements
{

public:
	GUIElements(const char* name);
	virtual ~GUIElements();

	void ToggleActive();
	bool IsActive() const;

	virtual void Draw() = 0;
	void Move();
	virtual bool CleanUp() {return true;};

	virtual bool Save(rapidjson::Value& Node, rapidjson::Document::AllocatorType& allocator) { return true; };
public:
	bool active = false;
	//int width = 1, height = 1, posx = 0, posy = 0;
	ImVec2 position;
	ImVec2 size;
	std::string name;


protected:
};

