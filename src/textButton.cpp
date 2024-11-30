#include "../include/button.h"
#include "../include/mainHeader.h"

int textButton::nextId = 0;

void textButton::loadText(SDL_Renderer* renderer) {
    if (textTexture != nullptr)
        SDL_DestroyTexture(textTexture); // destroy the texture to create a new one

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(textFont, text.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Cannot create surface for text, error:" << TTF_GetError() << "\n";
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); // creating the text texture that can be rendered
    SDL_FreeSurface(textSurface); // we don't want to make a thousand duplicates of a same surface and cause a memory leak, right?

    if (textTexture == nullptr) {
        std::cerr << "Cannot create text. Error: " << TTF_GetError() << "\n";
        return;
    }
}

void textButton::render(SDL_Renderer* renderer) {
    if (!isVisible()) {
        return;
    }
    // set the color to draw for the button and also set the settings to render the button
    SDL_Color drawColor = hovered ? hoverColor : buttonColor;
    // the color of the button will be the color of the hovered button when disabled (by default)
    if (!active) {
        drawColor = hoverColor;
    }
    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);
    // std::cout << SDL_GetError() << "whatever" << "\n";
    // std::cout << hovered << "\n";
    if (textTexture != nullptr) {
        int textWidth, textHeight;
        // querying the texture to attempt to align the text
        if (textTexture == nullptr) {
            std::cout << "Error: Texture is nullptr!" << std::endl;
        }
        else {
            SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        }
        // now create a rect for the text
        SDL_Rect textRect;

        // align the text
        switch (xAlign) {
        case LEFT:
            textRect.x = buttonRect.x + 5;
            break;
        case CENTER:
            textRect.x = buttonRect.x + (buttonRect.w - textWidth) / 2;
            break;
        case RIGHT:
            textRect.x = buttonRect.x + (buttonRect.w - textWidth) - 5;
            break;
        }
        // LEFT = up, CENTER = center; RIGHT = down.
        switch (yAlign) {
        case LEFT:
            textRect.y = buttonRect.y + 5;
            break;
        case CENTER:
            textRect.y = buttonRect.y + (buttonRect.h - textHeight) / 2;
            break;
        case RIGHT:
            textRect.y = buttonRect.y + (buttonRect.h - textHeight) - 5;
            break;
        }

        textRect.w = textWidth; textRect.h = textHeight;

        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }
}

void textButton::setAction(std::function<void()> actionFunction) {
    buttonAction = actionFunction;
}

void textButton::setHoverAction(std::function<void()> actionFunction) {
    hoverAction = actionFunction;
}

bool textButton::isClicked(int x, int y) {
    return (x > buttonRect.x &&
        x < (buttonRect.x + buttonRect.w) &&
        y > buttonRect.y &&
        y < (buttonRect.y + buttonRect.h));
}

void textButton::checkHover(int mouseX, int mouseY) {
    hovered = isClicked(mouseX, mouseY);
}

bool textButton::isVisible() const {
    return visible;
}
void textButton::toggleVisiblility(bool value) {
    visible = value;
}
void textButton::handleEvents(SDL_Event& e) {
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

void textButton::toggleActive(bool value) {
    active = value;
}
