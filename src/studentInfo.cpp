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
		LogMessage(
			std::wstring(L"Unable to open file ") + towchar(finalFileName.c_str()) + L" for writing.",
			LOGF_NAME
		);
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

void readStudentDataBinary(
	const std::string& filename,
	ButtonManager& stdBtnLst,
	SDL_Renderer* renderer,
	std::vector<Student>& students
) {
	std::string finalFileName = sanitizeFileName(filename, ".txt");
	std::ifstream inFile(finalFileName, std::ios::binary);
	if (!inFile.is_open()) {
		std::cerr << "Unable to open file for reading.\n";
		return;
	}

	students.clear();

	size_t nameLength;
	while (true) {
		size_t nameLength;

		if (!inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength))) { inFile.close(); return; }

		std::string fullName;
		try {
			fullName = std::string(nameLength, '\0');
		}
		catch (const std::exception& e) {
			LogMessage(
				L"Error: Corrupted/unsupported type format",
				LOGF_NAME
			);
		}
		if (!inFile.read(&fullName[0], nameLength)) { inFile.close(); return; }
		
		int number;
		if (!inFile.read(reinterpret_cast<char*>(&number), sizeof(number))) { inFile.close(); return; }

		std::string abbrName = abbreviateName(fullName, true);

		students.push_back({ abbrName, fullName, number });
		for (auto& button : stdBtnLst.buttons) {
			if (std::get<TextButton>(button).getId() != number) continue;
			std::get<TextButton>(button).text = abbrName;
			std::get<TextButton>(button).loadText(renderer);
		}
	}
	LogMessage(
		std::wstring(L"Successfully read data from ") + towchar(filename.c_str()),
		LOGF_NAME
	);
}