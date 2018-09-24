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
