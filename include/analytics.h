#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "database.h"
#include <vector>
#include <string>
#include <map>

class Analytics {
public:
    Analytics(std::shared_ptr<Database> db);

    // Reports
    void generateClassReport();
    void generateStudentCountReport();
    void generateDetailedReport();
    void displayDashboard();
    
    // Statistics
    int getTotalStudents();
    std::vector<std::pair<std::string, int>> getClassDistribution();
    std::string getMostCommonClass();
    int getStudentsInClass(const std::string& className);

private:
    std::shared_ptr<Database> database;
    void displayBox(const std::string& title, const std::string& content) const;
};

#endif // ANALYTICS_H
