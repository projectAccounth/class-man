#pragma once

// Include guard
#ifndef MAIN_HDR_H
#define MAIN_HDR_H

// These are the window width and height, can be changed
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Includes for the program

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <functional>
#include <variant>
#include <type_traits>
#include <fstream>
#include <algorithm>
#include <random>

const int tableRows = 4, tableColumns = 4;
const int tbSize = 60;
const int students = 30;

const SDL_Color defaultButtonColor(177, 177, 177, 255);
const SDL_Color hoveredButtonColor(122, 122, 122, 255);
const SDL_Color defaultTextColor(0, 0, 0, 255);

enum textAlign {
    LEFT,
    CENTER,
    RIGHT
};

#endif /* MAIN_HDR_H */
