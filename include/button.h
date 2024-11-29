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
    std::function<void()> hoverAction;
    
    // The "ID" of the button, can be used for querying.
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

    // Preloading text (must be called before rendering)
    void loadText(SDL_Renderer* renderer);

    // Basically just rendering the button on the specified renderer.
    void render(SDL_Renderer* renderer);

    // Setting the action for the button.
    void setAction(std::function<void()> actionFunction);

    // Setting the function responsible for hovering.
    void setHoverAction(std::function<void()> actionFunction);

    // Returns the visibility of the button.
    bool isVisible() const;

    // Alternative to button.visible = value : bool;
    void toggleVisiblility(bool value);

    // Handle all clicking and hovering events. Put in the PollEvent loop.
    void handleEvents(SDL_Event& event);

    // Alternative to button.active = value : bool;
    void toggleActive(bool value);

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

    SDL_Rect buttonRect;
    int id;
       
    bool hovered;
    bool visible, active;
    std::function<void()> hoverAction;
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

    // Renders the button.
    void render(SDL_Renderer* renderer);

    // Set an action (void function) for the button.
    void setAction(std::function<void()> actionFunction);

    // Setting the function responsible for hovering.
    void setHoverAction(std::function<void()> actionFunction);

    // Returns the visibility.
    bool isVisible() const;

    // Alternative to directly assigning button.visible.
    void toggleVisiblility(bool value);

    // Handles clicking and hovering. Put in the PollEvent loop.
    void handleEvents(SDL_Event& event);

    // Alternative to directly assigning button.active.
    void toggleActive(bool value);

    ~imageButton() {}
};


#endif /* BUTTON_H */

