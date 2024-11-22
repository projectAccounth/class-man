#pragma once

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "mainHeader.h"

class textBox {
protected:
    TTF_Font* textFont;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    bool visible;
    textAlign xAlign;
    textAlign yAlign;

    std::vector<std::string> lines;

    int lineHeight() const;

    std::vector<std::string> splitTextIntoLines(std::string &text, int maxWidth);

public:
    SDL_Rect textBoxRect;
    SDL_Color boxColor;
    std::string text;
    bool editable;

    SDL_Color textColor;
    textBox(SDL_Rect br, SDL_Color bc, std::string t,
            SDL_Color tc, TTF_Font* tf,
            textAlign alignX, textAlign alignY) :
        textBoxRect(br),
        boxColor(bc),
        text(t),
        textColor(tc),
        textFont(tf),
        visible(true),
        xAlign(alignX),
        yAlign(alignY) {}

    void render(SDL_Renderer* renderer);

    void update(SDL_Renderer* renderer, const char* textToUpdate);

    bool isVisible() const;

    void toggleVisibility(bool value);

    ~textBox() {}
};

class editableTextBox : public textBox {
private:
    size_t cursorPosition;

    void handleBackspace();
    void moveCursorLeft();
    void moveCursorRight();
    void handleDelete();

    void insertCharacter(char c);

    std::unordered_map<SDL_Keycode, std::function<void()>> keyActions;
public:
    std::string text;
    bool editable;
    editableTextBox(SDL_Rect br, SDL_Color bc, std::string t,
                    SDL_Color tc, TTF_Font* tf,
                    textAlign alignX, textAlign alignY, bool edbe) :
        textBox(br, bc, "", tc, tf, alignX, alignY),
        editable(edbe),
        text(""), cursorPosition(0) {
            // Define actions for specific keys
            keyActions[SDLK_BACKSPACE] = [this]() { handleBackspace(); };
            keyActions[SDLK_LEFT] = [this]() { moveCursorLeft(); };
            keyActions[SDLK_RIGHT] = [this]() { moveCursorRight(); };
            keyActions[SDLK_DELETE] = [this]() { handleDelete(); };
        }

    void render(SDL_Renderer* renderer);

    void reset();

    void handleEvent(const SDL_Event& e);
    std::string getText() const;
};

#endif /* TEXTBOX_H */