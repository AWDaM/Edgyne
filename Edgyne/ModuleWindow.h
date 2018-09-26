#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(rapidjson::Document& document);
	bool CleanUp();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);

	void SetFullscreen();
	void SetBorderless();
	void SetDesktopFullscreen();
	void Configuration();
	

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool desktop_fullscreen = false;

	int window_w;
	int window_h;
	float brightness;
};

#endif // __ModuleWindow_H__