#pragma once

#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include "mainHeader.h"

struct Student {
	std::string name;
	int number;
};

void saveStudentDataBinary(const Student& student, const std::string& filename);

void readStudentDataBinary(const std::string& filename);

#endif /* STUDENTINFO_H */