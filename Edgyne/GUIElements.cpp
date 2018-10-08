#include "GUIElements.h"



GUIElements::GUIElements(const char* name) : name(name)
{
}


GUIElements::~GUIElements()
{
}

void GUIElements::ToggleActive()
{
	active = !active;
}

bool GUIElements::IsActive() const
{
	return active;
}

void GUIElements::Move()
{
	if (ImGui::GetWindowPos() != position)
	{
		ImGui::GetWindowPos() = position;
	}
	if (ImGui::GetWindowSize() != size)
	{
		ImGui::GetWindowSize() != size;
	}
}
