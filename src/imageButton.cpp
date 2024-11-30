#include "../include/button.h"
#include "../include/mainHeader.h"

int imageButton::nextId = 0;

void imageButton::render(SDL_Renderer* renderer) {
    if (!isVisible()) {
        return;
    }

    SDL_Surface* imgSurf = IMG_Load(defaultImgPath);
    SDL_Surface* hoverSurf = IMG_Load(hoverImgPath);

    if (imgSurf == nullptr || hoverSurf == nullptr) {
        std::cout << "A problem occurred when creating one or more surface for image button. Error: " << SDL_GetError() << '\n';
        return;
    }

    hoverTexture = SDL_CreateTextureFromSurface(renderer, hoverSurf);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, imgSurf);

    if (hoverTexture == nullptr || buttonTexture == nullptr) {
        std::cout << "A problem occurred when creating one or more texture for image button. Error: " << SDL_GetError() << '\n';
        return;
    }

    SDL_Texture* finalTexture = hovered ? hoverTexture : buttonTexture;

    SDL_RenderCopy(renderer, finalTexture, nullptr, &buttonRect);

    SDL_DestroyTexture(finalTexture);
    SDL_DestroyTexture(hoverTexture);
    SDL_DestroyTexture(buttonTexture);

    SDL_FreeSurface(hoverSurf);
    SDL_FreeSurface(imgSurf);
}

void imageButton::setAction(std::function<void()> actionFunction) {
    buttonAction = actionFunction;
}

void imageButton::setHoverAction(std::function<void()> actionFunction) {
    hoverAction = actionFunction;
}

bool imageButton::isClicked(int x, int y) {
    return (x > buttonRect.x &&
        x < (buttonRect.x + buttonRect.w) &&
        y > buttonRect.y &&
        y < (buttonRect.y + buttonRect.h));
}
// too lazy to remove these foos
void imageButton::checkHover(int mouseX, int mouseY) {
    hovered = isClicked(mouseX, mouseY);
}

bool imageButton::isVisible() const {
    return visible;
}
void imageButton::toggleVisiblility(bool value) {
    visible = value;
}
void imageButton::handleEvents(SDL_Event& e) {
    int x, y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN && active && visible) {
        if (x > buttonRect.x && x < (buttonRect.x + buttonRect.w) &&
            y > buttonRect.y && y < (buttonRect.y + buttonRect.h)) {
            hovered = true;
            if (hoverAction) {
                hoverAction();
            }
        }
        else {
            hovered = false;
        }

        // if mouse is clicked while hovering, those couts are safe to remove
        if (e.type == SDL_MOUSEBUTTONDOWN && hovered) {
            if (buttonAction) {
                std::cout << "Button clicked!" << std::endl;
                buttonAction();
            }
            else {
                std::cout << "No action assigned!" << std::endl;
            }
        }
    }
}

void imageButton::toggleActive(bool value) {
    active = value;
}
