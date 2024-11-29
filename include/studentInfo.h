#pragma once

#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include "mainHeader.h"
#include "buttonManager.h"

struct Student {
	std::string name;
	std::string fullName;
	int number;
};

void saveStudentDataBinary(std::vector<Student>& students, const std::string& filename);

void readStudentDataBinary(const std::string& filename, buttonManager& stdBtnLst, SDL_Renderer* renderer, std::vector<Student>& students);

#endif /* STUDENTINFO_H */