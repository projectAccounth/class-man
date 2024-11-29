#include "../include/nameLocalization.h"

std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

bool isAbbreviated(const std::string& word) {
    return word.size() > 1 && word.back() == '.' && std::isupper(word[0]);
}

std::string cleanWord(const std::string& word) {
    std::string cleaned;
    for (char ch : word) {
        if (std::isalpha(ch) || ch == '.') {
            cleaned += ch;
        }
    }
    return cleaned;
}
std::string abbreviateName(const std::string& fullName, bool isEasternOrder) {
    std::vector<std::string> words = split(fullName);
    std::vector<std::string> cleanedWords;

    // Clean and validate words
    for (const std::string& word : words) {
        std::string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            cleanedWords.push_back(cleaned);
        }
    }

    size_t wordCount = cleanedWords.size();
    if (wordCount < 2) {
        // Return as is for single-word names
        return fullName;
    }

    if (isEasternOrder) {
        // Eastern Order: Abbreviate the first word, keep the last word
        std::string surnameAbbrev = isAbbreviated(cleanedWords[0]) ? cleanedWords[0] : std::string(1, cleanedWords[0][0]) + ".";
        return surnameAbbrev + " " + cleanedWords.back();
    }
    else {
        // Western Order: Keep the first word, abbreviate the last word
        std::string lastNameAbbrev = isAbbreviated(cleanedWords.back()) ? cleanedWords.back() : std::string(1, cleanedWords.back()[0]) + ".";
        return cleanedWords[0] + " " + lastNameAbbrev;
    }
    return fullName;
}