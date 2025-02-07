#pragma once

#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include "mainHeader.h"
#include "ButtonManager.h"

struct Student {
	std::string name;
	std::string fullName;
	int number;
};

void saveStudentDataBinary(std::vector<Student>& students, const std::string& filename);

void readStudentDataBinary(const std::string& filename, ButtonManager& stdBtnLst, SDL_Renderer* renderer, std::vector<Student>& students);

#endif /* STUDENTINFO_H */