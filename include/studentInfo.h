#pragma once

#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include "mainHeader.h"
#include "buttonManager.h"

struct Student {
	std::string name;
	int number;
};

void saveStudentDataBinary(std::vector<Student> students, const std::string& filename);

void readStudentDataBinary(const std::string& filename, buttonManager& stdBtnLst, SDL_Renderer* renderer);

#endif /* STUDENTINFO_H */