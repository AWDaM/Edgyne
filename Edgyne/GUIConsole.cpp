#include "GUIConsole.h"
#include "Application.h"
#include "ModuleInput.h"



GUIConsole::GUIConsole(std::string log, bool active = false) : GUIElements("console",active)
{
	position = { 420, 200 };
	size = { 600, 300 };

	//canLog = true;
	//AddLog(log.data());
}


GUIConsole::~GUIConsole()
{
}

void GUIConsole::Draw()
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	
		Move();
		ImGui::TextUnformatted(Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::End();
	
}


bool GUIConsole::Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator)
{
	Node.AddMember("active", active, allocator);

	return true;
}

bool GUIConsole::Load(rapidjson::Value& Node)
{
	active = Node["active"].GetBool();
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