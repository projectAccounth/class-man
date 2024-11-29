#include "../include/studentInfo.h"
#include "../include/nameLocalization.h"

std::string sanitizeFileName(const std::string& filename, const std::string& defaultExt) {
	const std::string invalidChars = R"(\ / : * ? " < > |)";

	std::string sanitized = filename;

	sanitized.erase(
		std::remove_if(sanitized.begin(), sanitized.end(),
			[&invalidChars](char c) {
				return invalidChars.find(c) != std::string::npos;
			}
		),
		sanitized.end()
	);

	size_t dotPos = sanitized.find_last_of('.');
	bool hasExt = (dotPos != std::string::npos && dotPos != 0 && dotPos != sanitized.size() - 1);

	if (!hasExt) {
		sanitized += defaultExt;
	}

	return sanitized;
}



void saveStudentDataBinary(std::vector<Student>& students, const std::string& filename) {
	std::string finalFileName = sanitizeFileName(filename, ".txt");
	std::fstream outFile(finalFileName.c_str(), std::ios::binary | std::ios::out | std::ios::trunc); // Open in binary append mode
	if (!outFile.is_open()) {
		std::cerr << "Name: " << finalFileName << '\n';
		std::cerr << "Unable to open file for writing.\n";
		return;
	}
	for (auto& student : students) {
		size_t nameLength = student.fullName.size();
		outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength)); // Write name length
		outFile.write(student.fullName.c_str(), nameLength); // Write name
		outFile.write(reinterpret_cast<const char*>(&student.number), sizeof(student.number)); // Write number
	}
	outFile.close();
}

void readStudentDataBinary(const std::string& filename, buttonManager& stdBtnLst, SDL_Renderer* renderer, std::vector<Student>& students) {
	std::string finalFileName = sanitizeFileName(filename, ".txt");
	std::ifstream inFile(finalFileName, std::ios::binary);
	if (!inFile.is_open()) {
		std::cerr << "Unable to open file for reading.\n";
		return;
	}

	students.clear();

	size_t nameLength;
	while (inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength))) {
		std::string fullName(nameLength, ' ');
		inFile.read(&fullName[0], nameLength);
		if (!inFile) break;

		int number;
		inFile.read(reinterpret_cast<char*>(&number), sizeof(number));
		if (!inFile) break;

		std::string abbrName = abbreviateName(fullName, true);

		students.push_back({ abbrName, fullName, number });
		for (auto& button : stdBtnLst.buttons) {
			std::visit([&](auto& btn) {
				if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
					if (btn.id == number) {
						btn.text = abbrName;
						btn.loadText(renderer);
					}
				}
			}, button);
		}
	}
}