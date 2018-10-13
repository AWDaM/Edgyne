#include "GUIConsole.h"
#include "Application.h"
#include "ModuleInput.h"



GUIConsole::GUIConsole(std::string log) : GUIElements("console")
{
	width = 658;
	height = 300;
	posy = 100;

	//canLog = true;
	//AddLog(log.data());
}


GUIConsole::~GUIConsole()
{
}

void GUIConsole::Draw()
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}

bool GUIConsole::CleanUp()
{
	//App->canLog = false;
	return true;
}

bool GUIConsole::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	return true;
}



void GUIConsole::AddLog(const char* entry)
{
	Buf.clear();
	Buf.appendf(entry);
	ScrollToBottom = true;
}

void GUIConsole::Clear()
{
	Buf.clear();
}