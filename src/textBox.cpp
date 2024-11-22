#include "../include/textBox.h"

int textBox::lineHeight() const {
    int fontHeight = TTF_FontHeight(textFont);
    return fontHeight;
}

std::vector<std::string> textBox::splitTextIntoLines(std::string& text, int maxWidth) {
    std::vector<std::string> outLines;
    std::string currentLine;
    std::string word;
    int lineWidth = 0;

    for (char c : text) {
        if (c == ' ' || c == '\n') {
            // line measurement
            int wordWidth = 0, wordHeight = 0;
            TTF_SizeText(textFont, (currentLine + word).c_str(), &wordWidth, &wordHeight);

            if (wordWidth > maxWidth) {
                // wrap the text
                outLines.push_back(currentLine);
                currentLine = word;
            }
            else {
                currentLine += word;
            }

            if (c == '\n') {
                outLines.push_back(currentLine);
                currentLine.clear();
            }
            else {
                currentLine += ' ';
            }
            word.clear();
        }
        else {
            word += c;
        }
    }

    // last line!!
    if (!currentLine.empty() || !word.empty()) {
        outLines.push_back(currentLine + word);
    }
    return outLines;
}

void textBox::render(SDL_Renderer* renderer) {
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
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(textFont, line.c_str(), textColor);
        if (!textSurface) continue;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect destRect = { startX, startY + offsetY, textWidth, textHeight };
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

        offsetY += textHeight;
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void textBox::update(SDL_Renderer* renderer, const char* textToUpdate) {
    text = textToUpdate;
    render(renderer);
}

bool textBox::isVisible() const {
    return visible;
}

void textBox::toggleVisibility(bool value) {
    visible = value;
}