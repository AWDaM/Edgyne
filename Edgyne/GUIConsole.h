#pragma once

#include "GUIElements.h"


class GUIConsole : public GUIElements
{
public:

	GUIConsole(std::string log, bool active);
	virtual ~GUIConsole();

	void Draw() override;
	bool CleanUp() override;

	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);
	bool Load(rapidjson::Value& Node);

	void Clear();
	void AddLog(const char* entry);

public:
	//bool canLog = false;
	ImGuiTextBuffer Buf;
	bool ScrollToBottom;
};

