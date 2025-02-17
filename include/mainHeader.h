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
#include <cctype>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <sstream>
#include <lfmain.h>

#include "logging.h"

#define LOGF_NAME "outlog.txt"

const int tableRows = 4, tableColumns = 4;
const int tbSize = 60;
const int studentsCount = 30;

const SDL_Color defaultButtonColor = SDL_Color{ 177, 177, 177, 255 };
const SDL_Color hoveredButtonColor = SDL_Color{ 122, 122, 122, 255 };
const SDL_Color defaultTextColor = SDL_Color{ 0, 0, 0, 255 };

#endif /* MAIN_HDR_H */
