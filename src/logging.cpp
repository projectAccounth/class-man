#include "../include/logging.h"

const wchar_t* towchar(const char* str) {
    if (!str) return nullptr;

    size_t len = std::mbstowcs(nullptr, str, 0);
    if (len == static_cast<size_t>(-1)) return nullptr;

    static std::vector<wchar_t> buffer(len + 1);
    std::mbstowcs(buffer.data(), str, len + 1);

    return buffer.data();
}

void LogMessage(const std::wstring& message, const std::string& LOG_FILE, bool verboseMode) {
    std::wofstream log(LOG_FILE, std::ios::app);
    if (!log) return;

    time_t now = time(0);
    struct tm localTime;
    localtime_s(&localTime, &now);

    wchar_t timeBuffer[20];
    wcsftime(timeBuffer, sizeof(timeBuffer) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &localTime);

    log << L"[" << timeBuffer << L"] " << message << std::endl;

    if (verboseMode) {
        std::wcout << message << std::endl;
    }
}