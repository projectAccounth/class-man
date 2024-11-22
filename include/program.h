#pragma once

// Include guard
#ifndef PROGRAM_H
#define PROGRAM_H

#include "mainHeader.h"

class Program {
public:
	SDL_Renderer* createRenderer(SDL_Window* targetWindow);

	SDL_Window* createWindow(int height, int width, bool isFullScreen, const char* title);

	void onQuit(SDL_Window* window, SDL_Renderer* renderer);

	// All program controls go here
	void programControls();

	bool processEvent(SDL_Event event);
};

#endif /* PROGRAM_H */
