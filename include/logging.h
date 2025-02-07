#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>


void LogMessage(const std::wstring& message, const std::string& LOG_FILE, bool verboseMode = false);

const wchar_t* towchar(const char* str);