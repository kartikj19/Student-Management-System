#include "student.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Student::Student() : rollNo(0), name(""), className(""), createdAt(time(nullptr)), updatedAt(time(nullptr)) {}

Student::Student(int rollNo, const std::string& name, const std::string& className)
    : rollNo(rollNo), name(name), className(className), createdAt(time(nullptr)), updatedAt(time(nullptr)) {}

void Student::display() const {
    std::cout << "\n" << std::setw(15) << rollNo 
              << std::setw(25) << name 
              << std::setw(15) << className;
}

std::string Student::toString() const {
    std::ostringstream oss;
    oss << "Roll No: " << rollNo << ", Name: " << name << ", Class: " << className;
    return oss.str();
}
