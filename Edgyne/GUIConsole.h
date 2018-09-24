#pragma once

#include "GUIElements.h"
#include "Imgui/imgui.h"

class GUIConsole : public GUIElements
{
public:
	GUIConsole();
	virtual ~GUIConsole();

	void Draw() override;
	bool CleanUp() override;

	void Clear();
	void AddLog(const char* entry);

public:
	bool canLog = false;
	ImGuiTextBuffer Buf;
	bool ScrollToBottom;
};

