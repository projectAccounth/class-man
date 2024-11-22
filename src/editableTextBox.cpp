#include "../include/textBox.h"

void editableTextBox::render(SDL_Renderer* renderer) {
    int padding = 5;
    int maxWidth = textBoxRect.w - padding * 2;

    int totalHeight = lines.size() * lineHeight();
    int startY = textBoxRect.y;

    if (!isVisible()) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    SDL_RenderFillRect(renderer, &textBoxRect);

    // checking whether the text is empty or not to prevent problematic stuff
    if (text.empty()) {
        return;
    }

    lines = splitTextIntoLines(text, maxWidth);

    // LEFT = up, CENTER = center; RIGHT = down.
    switch (yAlign) {
    case LEFT:
        startY = textBoxRect.y + padding;
        break;
    case CENTER:
        startY = textBoxRect.y + (textBoxRect.h - totalHeight) / 2;
        break;
    case RIGHT:
        startY = textBoxRect.y + (textBoxRect.h - totalHeight) - padding;
        break;
    }

    int offsetY = 0;

    switch (yAlign) {
    case LEFT:
        startY = textBoxRect.y + padding;
        break;
    case CENTER:
        startY = textBoxRect.y + (textBoxRect.h - totalHeight) / 2;
        break;
    case RIGHT:
        startY = textBoxRect.y + (textBoxRect.h - totalHeight) - padding;
        break;
    }

    for (const auto& line : lines) {
        int textWidth = 0, textHeight = 0;
        TTF_SizeText(textFont, line.c_str(), &textWidth, &textHeight);

        int startX = textBoxRect.x;
        switch (xAlign) {
        case LEFT:
            startX = textBoxRect.x + padding;
            break;
        case CENTER:
            startX = textBoxRect.x + (maxWidth - textWidth) / 2;
            break;
        case RIGHT:
            startX = textBoxRect.x + maxWidth - textWidth - padding;
            break;
        }

        SDL_Surface* textSurface = TTF_RenderText_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void editableTextBox::handleEvent(const SDL_Event& e) {
    if (!editable) return;
    if (e.type == SDL_KEYDOWN) {
        auto it = keyActions.find(e.key.keysym.sym);
        if (it != keyActions.end()) {
            it->second(); // Call the corresponding action
        }
    }
    else if (e.type == SDL_TEXTINPUT) {
        insertCharacter(e.text.text[0]); // Insert the input character
    }
}

void editableTextBox::handleBackspace() {
    if (cursorPosition > 0) {
        text.erase(cursorPosition - 1, 1);
        cursorPosition--;
    }
}

void editableTextBox::handleDelete() {
    if (cursorPosition < text.size()) {
        text.erase(cursorPosition, 1);
    }
}

void editableTextBox::moveCursorLeft() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}

void editableTextBox::moveCursorRight() {
    if (cursorPosition < text.size()) {
        cursorPosition++;
    }
}

void editableTextBox::insertCharacter(char c) {
    text.insert(cursorPosition, 1, c);
    cursorPosition++;
}

std::string editableTextBox::getText() const {
    return text;
}

void editableTextBox::reset() {
    text.clear();
    cursorPosition = 0;
}