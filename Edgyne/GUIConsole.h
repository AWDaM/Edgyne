#ifndef __GUI_CONSOLE_H__
#define __GUI_CONSOLE_H__



#include "GUIElements.h"


class GUIConsole : public GUIElements
{
public:

	GUIConsole(std::string log, bool active);
	~GUIConsole();

	void Draw();

	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
	bool Load(rapidjson::Value& Node);

	void Clear();
	void AddLog(const char* entry);

public:
	//bool canLog = false;
	ImGuiTextBuffer Buf;
	bool ScrollToBottom;
};

#endif // !__GUI_CONSOLE_H__