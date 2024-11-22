#include "../include/studentInfo.h"

std::string sanitizeFileName(const std::string& filename) {
	// List of invalid characters
	const std::string invalidChars = R"(\ / : * ? " < > |)";

	// Copy the input filename
	std::string sanitized = filename;

	// Remove invalid characters
	sanitized.erase(
		std::remove_if(sanitized.begin(), sanitized.end(),
			[&invalidChars](char c) {
				return invalidChars.find(c) != std::string::npos;
			}
		),
		sanitized.end()
	);

	// Return the sanitized filename
	return sanitized;
}


void saveStudentDataBinary(const Student& student, const std::string& filename) {
	std::string finalFileName = sanitizeFileName(filename);
	std::ofstream outFile(finalFileName, std::ios::binary | std::ios::app | std::ios::out); // Open in binary append mode
	if (outFile.is_open()) {
		size_t nameLength = student.name.size();
		outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength)); // Write name length
		outFile.write(student.name.c_str(), nameLength); // Write name
		outFile.write(reinterpret_cast<const char*>(&student.number), sizeof(student.number)); // Write number
		outFile.close();
	}
	else {
		std::cerr << "Unable to open file for writing.\n";
	}
}

void readStudentDataBinary(const std::string& filename) {
	std::string finalFileName = sanitizeFileName(filename);
	std::ifstream inFile(finalFileName, std::ios::binary);
	if (inFile.is_open()) {
		while (true) {
			size_t nameLength;
			inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength)); // Read name length
			if (inFile.eof()) break; // End of file

			std::string name(nameLength, ' ');
			inFile.read(&name[0], nameLength); // Read name

			int number;
			inFile.read(reinterpret_cast<char*>(&number), sizeof(number)); // Read number

			std::cout << "Student Name: " << name << ", Number: " << number << "\n";
		}
		inFile.close();
	}
	else {
		std::cerr << "Unable to open file for reading.\n";
	}
}