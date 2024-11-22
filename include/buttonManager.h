#pragma once

#ifndef BUTTONMGR_H
#define BUTTONMGR_H

#include "mainHeader.h"
#include "button.h"

using buttonType = std::variant<textButton, imageButton>;

class buttonManager {
public:
	std::vector<buttonType> buttons;

	void add(buttonType btn);

	void renderAll(SDL_Renderer *renderer);

	void handleAllEvent(SDL_Event e);

	void loadAllText(SDL_Renderer *renderer);
};

#endif /* BUTTONMGR_H */