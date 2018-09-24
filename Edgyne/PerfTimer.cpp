// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfTimer::frequency = 0;

// ---------------------------------------------
PerfTimer::PerfTimer()
{
	if(!frequency) frequency = SDL_GetPerformanceFrequency();
	Start();
}

// ---------------------------------------------
void PerfTimer::Start()
{
	if (!isPaused)
		started_at = paused_at = SDL_GetPerformanceCounter();
	else
		started_at = SDL_GetPerformanceCounter() - paused_at;

	isPaused = false;
}

// ---------------------------------------------
double PerfTimer::ReadMs() const
{
	if (!isPaused)
		return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
	else
		return 1000.0 * (double(paused_at) / double(frequency));
}

float PerfTimer::ReadSec() const
{
	if (!isPaused)
		return (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
	else
		return (double(paused_at) / double(frequency));
}

// ---------------------------------------------
uint64 PerfTimer::ReadTicks() const
{
	if (!isPaused)
		return SDL_GetPerformanceCounter() - started_at;
	else
		return paused_at;
}

void PerfTimer::PauseTimer()
{
	if (!isPaused)
	{
		paused_at = SDL_GetPerformanceCounter() - started_at;
		isPaused = true;
	}
}


