#ifndef EXPORTER_H
#define EXPORTER_H

#include "student.h"
#include <vector>
#include <string>

class Exporter {
public:
    Exporter();

    // Export formats
    static bool exportToCSV(const std::vector<Student>& students, const std::string& filePath);
    static bool exportToJSON(const std::vector<Student>& students, const std::string& filePath);
    static bool exportToTXT(const std::vector<Student>& students, const std::string& filePath);

private:
    static std::string escapeCSV(const std::string& field);
    static std::string escapeJSON(const std::string& str);
};

#endif // EXPORTER_H
