#include "../include/program.h"
#include "../include/mainHeader.h"

SDL_Renderer* Program::createRenderer(SDL_Window *targetWindow) {
	return SDL_CreateRenderer(targetWindow, -1, SDL_RENDERER_PRESENTVSYNC);
}

SDL_Window* Program::createWindow(int height, int width, bool isFullScreen, const char* title) {

	SDL_Window *targetWindow = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height, 0);

	if (targetWindow == NULL) {
		std::cout << ("Could not create window: %s\n", SDL_GetError());
		exit(1);
	}
	
	// If you want to go to fullscreen mode by default, pass "true" as
	// the argument to isFullScreen
	if (isFullScreen) {
		SDL_SetWindowFullscreen(targetWindow, SDL_WINDOW_FULLSCREEN);
	}
	return targetWindow;
}


void Program::onQuit(SDL_Window *window, SDL_Renderer *renderer) {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Program::programControls() {
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	// Add more controls by using if (keyState[SDL_SCANCODE_KEY]) // ... or switch-case.
	if (keyState[SDL_SCANCODE_A]) {
		std::cout << "A" << '\n';
	}
}

bool Program::processEvent(SDL_Event event) {
	bool isRunning = true;
	switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE: {
			isRunning = false;
			break;
		}
		case SDL_QUIT: {
			isRunning = false;
			break;
		}
	}

	// returning the boolean
	return isRunning;
}