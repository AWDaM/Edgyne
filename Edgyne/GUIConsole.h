#pragma once

#include "GUIElements.h"


class GUIConsole : public GUIElements
{
public:

	GUIConsole(std::string log);
	virtual ~GUIConsole();

	void Draw() override;
	bool CleanUp() override;

	void Clear();
	void AddLog(const char* entry);

public:
	//bool canLog = false;
	ImGuiTextBuffer Buf;
	bool ScrollToBottom;
};

