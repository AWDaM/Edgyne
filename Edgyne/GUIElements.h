#pragma once
#include "Globals.h"
#include <string>

class GUIElements
{

public:
	GUIElements(const char* name);
	virtual ~GUIElements();

	void ToggleActive();
	bool IsActive() const;

	virtual void Draw() = 0;
	virtual bool CleanUp() {return true;};

public:
	bool active = false;
	int width = 1, height = 1, posx = 0, posy = 0;

protected:
	std::string name;
};

