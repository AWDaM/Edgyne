#include "GUIConsole.h"
#include "Application.h"
#include "ModuleInput.h"



GUIConsole::GUIConsole() : GUIElements("console")
{
	width = 658;
	height = 300;
	posy = 100;
	active = true;
	canLog = true;
}


GUIConsole::~GUIConsole()
{
}

void GUIConsole::Draw()
{
	LOG("test LOG on an update");
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}

bool GUIConsole::CleanUp()
{
	App->canLog = false;
	return true;
}



void GUIConsole::AddLog(const char * entry)
{
	if (canLog)
	{
		Buf.appendf(entry);
		ScrollToBottom = true;
	}
}

void GUIConsole::Clear()
{
	Buf.clear();
}