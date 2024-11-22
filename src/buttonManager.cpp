#include "../include/buttonManager.h"

void buttonManager::add(buttonType btn) {
	buttons.push_back(btn);
}

void buttonManager::renderAll(SDL_Renderer *renderer) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) { btn.render(renderer); }, button);
	}
}

void buttonManager::handleAllEvent(SDL_Event e) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) { btn.handleEvents(e); }, button);
	}
}

void buttonManager::loadAllText(SDL_Renderer *renderer) {
	for (auto& button : buttons) {
		std::visit([&](auto& btn) {
			if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
				btn.loadText(renderer);
			}
		}, button);
	}
}