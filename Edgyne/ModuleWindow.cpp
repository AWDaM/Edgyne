#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		 window_w = SCREEN_WIDTH * SCREEN_SIZE;
		 window_h = SCREEN_HEIGHT * SCREEN_SIZE;
		 brightness = 1.0f;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1 not rly now using 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			fullscreen = true;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
			resizable = true;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
			borderless = true;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			desktop_fullscreen = true;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_w, window_h, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
			SDL_SetWindowMinimumSize(window, MINIMUM_SCREEN_WIDHT, MINIMUM_SCREEN_HEIGHT);
			SDL_SetWindowMaximumSize(window, MAXIMUM_SCREEN_WIDHT, MAXIMUM_SCREEN_HEIGHT);
			SDL_SetWindowBrightness(window, brightness);
		}
		
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen()
{
	if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetBorderless()
{

	SDL_SetWindowBordered(window, (SDL_bool)borderless);

}

void ModuleWindow::SetDesktopFullscreen()
{
	if (desktop_fullscreen)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::Configuration()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			SetFullscreen();
		if (ImGui::Checkbox("Bordeless", &borderless))
			SetBorderless();
		if (ImGui::Checkbox("Fullscreen Desktop", &desktop_fullscreen))
			SetDesktopFullscreen();

		int min_w, min_h, max_w, max_h;

		SDL_GetWindowMinimumSize(window, &min_w, &min_h);
		SDL_GetWindowMaximumSize(window, &max_w, &max_h);

		if (ImGui::SliderInt("Width", &window_w, min_w, max_w))
			SDL_SetWindowSize(window, window_w, window_h);
		if(ImGui::SliderInt("Height", &window_h,min_h,max_h))
			SDL_SetWindowSize(window, window_w, window_h);
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f));
			SDL_SetWindowBrightness(window, brightness);

	}

}
