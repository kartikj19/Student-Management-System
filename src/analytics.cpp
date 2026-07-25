#include "analytics.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Analytics::Analytics(std::shared_ptr<Database> db) : database(db) {}

void Analytics::generateClassReport() {
    std::cout << "\n\n";
    displayBox("CLASS DISTRIBUTION REPORT", "");
    
    auto classDistribution = getClassDistribution();
    
    std::cout << "\n" << std::left << std::setw(20) << "Class Name" << std::setw(15) << "Student Count" << "\n";
    std::cout << std::string(35, '-') << "\n";
    
    int totalStudents = 0;
    for (const auto& [className, count] : classDistribution) {
        std::cout << std::left << std::setw(20) << className << std::setw(15) << count << "\n";
        totalStudents += count;
    }
    
    std::cout << std::string(35, '-') << "\n";
    std::cout << std::left << std::setw(20) << "TOTAL" << std::setw(15) << totalStudents << "\n";
}

void Analytics::generateStudentCountReport() {
    int total = getTotalStudents();
    
    std::cout << "\n\n";
    displayBox("STUDENT COUNT REPORT", "");
    
    std::cout << "\n  Total Students in System: " << total << "\n";
}

void Analytics::generateDetailedReport() {
    std::cout << "\n\n";
    displayBox("DETAILED STATISTICS", "");
    
    int total = getTotalStudents();
    auto classDistribution = getClassDistribution();
    
    std::cout << "\n  Total Students: " << total << "\n";
    std::cout << "  Total Classes: " << classDistribution.size() << "\n";
    std::cout << "  Most Common Class: " << getMostCommonClass() << "\n\n";
    
    std::cout << "  Classes:\n";
    for (const auto& [className, count] : classDistribution) {
        double percentage = (total > 0) ? (count * 100.0 / total) : 0;
        std::cout << "    " << std::left << std::setw(20) << className 
                  << count << " student(s) (" << std::fixed << std::setprecision(1) << percentage << "%)\n";
    }
}

void Analytics::displayDashboard() {
    std::cout << "\n\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << std::setw(50) << "STUDENT MANAGEMENT SYSTEM - DASHBOARD" << "\n";
    std::cout << std::string(50, '=') << "\n\n";
    
    int total = getTotalStudents();
    auto classDistribution = getClassDistribution();
    
    std::cout << "  [Statistics]\n";
    std::cout << "    Total Students: " << total << "\n";
    std::cout << "    Total Classes: " << classDistribution.size() << "\n";
    if (!classDistribution.empty()) {
        std::cout << "    Most Common Class: " << getMostCommonClass() << "\n";
    }
    
    std::cout << "\n  [Class Breakdown]\n";
    for (const auto& [className, count] : classDistribution) {
        std::cout << "    " << std::left << std::setw(20) << className << "" << count << " student(s)\n";
    }
    
    std::cout << "\n" << std::string(50, '=') << "\n\n";
}

int Analytics::getTotalStudents() {
    return database->getTotalStudents();
}

std::vector<std::pair<std::string, int>> Analytics::getClassDistribution() {
    return database->getStudentsPerClass();
}

std::string Analytics::getMostCommonClass() {
    auto distribution = getClassDistribution();
    if (distribution.empty()) return "N/A";
    
    return std::max_element(distribution.begin(), distribution.end(),
                           [](const auto& a, const auto& b) { return a.second < b.second; })->first;
}

int Analytics::getStudentsInClass(const std::string& className) {
    auto distribution = getClassDistribution();
    for (const auto& [className_, count] : distribution) {
        if (className_ == className) return count;
    }
    return 0;
}

void Analytics::displayBox(const std::string& title, const std::string& content) {
    std::cout << std::string(50, '-') << "\n";
    std::cout << "  " << std::left << std::setw(46) << title << "\n";
    std::cout << std::string(50, '-') << "\n";
    if (!content.empty()) {
        std::cout << content << "\n";
    }
}
