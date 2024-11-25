#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "mainHeader.h"

class textButton {
private:
    static int nextId;

    bool isClicked(int x, int y);

    void checkHover(int mouseX, int mouseY);
public:
    // dimension, size, position and initial RGB color for the button
    SDL_Rect buttonRect;
    SDL_Color buttonColor;

    SDL_Color textColor; // color for textButton
    SDL_Texture* textTexture;
    SDL_Color hoverColor; // color for hovering

    TTF_Font* textFont; // font for the text in the textButton
    std::string text; // text for the textButton

    textAlign xAlign, yAlign;

    bool hovered;
    bool visible, active;
    std::function<void()> buttonAction;
    
    int id;

    textButton(int x, int y, int w, int h,
        SDL_Color c, const char* t,
        SDL_Color tc, SDL_Color hc, TTF_Font* f,
        textAlign alignX, textAlign alignY)
        : buttonRect{ x, y, w, h },
        buttonColor(c),
        textColor(tc),
        textFont(f),
        text(t),
        xAlign(alignX),
        yAlign(alignY),
        textTexture(nullptr),
        hovered(false),
        hoverColor(hc),
        visible(true),
        active(true),
        id(nextId++) {}
    /* you don't have to care much about these, this is just the constructor for the class */

    void loadText(SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);

    void setAction(std::function<void()> actionFunction);

    bool isVisible() const;

    void toggleVisiblility(bool value);

    void handleEvents(SDL_Event& event);

    void toggleActive(bool value);

    // destructor for the class
    ~textButton() {}
};

class imageButton {
private:
    SDL_Texture* buttonTexture;
    SDL_Texture* hoverTexture; // texture for hovering, set hoverTexture to buttonTexture if you don't want hover.
    static int nextId;

    bool isClicked(int x, int y);

    void checkHover(int mouseX, int mouseY);
public:
    // dimension, size, position and initial RGB color for the button
    SDL_Rect buttonRect;
    int id;
       
    bool hovered;
    bool visible, active;
    std::function<void()> buttonAction;
    const char* defaultImgPath;
    const char* hoverImgPath;

    imageButton(int x, int y, int w, int h, std::string defaultImageFilePath, std::string hoverImageFilePath)
        : buttonRect{ x, y, w, h },
        hovered(false),
        buttonTexture(NULL),
        hoverTexture(NULL),
        visible(true),
        active(true),
        defaultImgPath(defaultImageFilePath.c_str()),
        hoverImgPath(hoverImageFilePath.c_str()),
        id(nextId++)
        {}

    void render(SDL_Renderer* renderer);

    void setAction(std::function<void()> actionFunction);

    bool isVisible() const;

    void toggleVisiblility(bool value);

    void handleEvents(SDL_Event& event);

    void toggleActive(bool value);

    // destructor for the class
    ~imageButton() {}
};


#endif /* BUTTON_H */

