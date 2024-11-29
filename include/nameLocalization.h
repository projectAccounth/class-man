#pragma once

#ifndef NAMELOCALIZATION_H
#define NAME_LOCALIZATION_H

#include "mainHeader.h"

// Basically just string.split(' ') equivalent
std::vector<std::string> split(const std::string& str);

// Abbrevation check (helper function)
bool isAbbreviated(const std::string& word);

// Clean up invalid words (helper function)
std::string cleanWord(const std::string& word);

// Function to abbreviate the name with localization support (the main func we'll be working w/)
// Returns the initial name if a wrong input is provided
std::string abbreviateName(const std::string& fullName, bool isEasternOrder);

#endif /* NAME_LOCALIZATION_H */
