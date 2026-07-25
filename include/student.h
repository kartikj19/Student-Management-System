#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <ctime>
#include <iostream>

class Student {
public:
    Student();
    Student(int rollNo, const std::string& name, const std::string& className);

    // Getters
    int getRollNo() const { return rollNo; }
    const std::string& getName() const { return name; }
    const std::string& getClassName() const { return className; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setRollNo(int no) { rollNo = no; }
    void setName(const std::string& n) { name = n; }
    void setClassName(const std::string& c) { className = c; }
    void setUpdatedAt() { updatedAt = time(nullptr); }

    // Display
    void display() const;
    std::string toString() const;

private:
    int rollNo;
    std::string name;
    std::string className;
    time_t createdAt;
    time_t updatedAt;
};

#endif // STUDENT_H
