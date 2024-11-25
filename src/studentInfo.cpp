#include "../include/studentInfo.h"

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



void saveStudentDataBinary(std::vector<Student> students, const std::string& filename) {
	std::string finalFileName = sanitizeFileName(filename, ".txt");
	std::fstream outFile(finalFileName.c_str(), std::ios::binary | std::ios::out | std::ios::trunc); // Open in binary append mode
	if (!outFile.is_open()) {
		std::cerr << "Name: " << finalFileName << '\n';
		std::cerr << "Unable to open file for writing.\n";
		return;
	}
	for (auto& student : students) {
		size_t nameLength = student.name.size();
		outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength)); // Write name length
		outFile.write(student.name.c_str(), nameLength); // Write name
		outFile.write(reinterpret_cast<const char*>(&student.number), sizeof(student.number)); // Write number
	}
	outFile.close();
}

void readStudentDataBinary(const std::string& filename, buttonManager& stdBtnLst, SDL_Renderer* renderer) {
	std::string finalFileName = sanitizeFileName(filename, ".txt");
	std::ifstream inFile(finalFileName, std::ios::binary);
	if (!inFile.is_open()) {
		std::cerr << "Unable to open file for reading.\n";
		return;
	}

	size_t nameLength;
	while (inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength))) {
		std::string name(nameLength, ' ');
		inFile.read(&name[0], nameLength); // Read name
		if (!inFile) break; // Ensure name read was successful

		int number;
		inFile.read(reinterpret_cast<char*>(&number), sizeof(number)); // Read number
		if (!inFile) break; // Ensure number read was successful

		for (auto& button : stdBtnLst.buttons) {
			std::visit([&](auto& btn) {
				if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
					if (btn.id == number) {
						btn.text = name;
						btn.loadText(renderer);
					}
				}
			}, button);
		}
	}
}