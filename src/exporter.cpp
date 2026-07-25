#include "exporter.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

Exporter::Exporter() {}

bool Exporter::exportToCSV(const std::vector<Student>& students, const std::string& filePath) {
    try {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            Logger::getInstance().logError("Failed to open file for CSV export: " + filePath);
            return false;
        }

        // Write header
        file << "Roll No,Name,Class\n";

        // Write data
        for (const auto& student : students) {
            file << student.getRollNo() << ","
                 << escapeCSV(student.getName()) << ","
                 << escapeCSV(student.getClassName()) << "\n";
        }

        file.close();
        Logger::getInstance().logAction("EXPORT_CSV", "Exported " + std::to_string(students.size()) + " records to " + filePath);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().logError("CSV export failed: " + std::string(e.what()));
        return false;
    }
}

bool Exporter::exportToJSON(const std::vector<Student>& students, const std::string& filePath) {
    try {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            Logger::getInstance().logError("Failed to open file for JSON export: " + filePath);
            return false;
        }

        file << "{\n  \"students\": [\n";

        for (size_t i = 0; i < students.size(); ++i) {
            const auto& student = students[i];
            file << "    {\n"
                 << "      \"rollNo\": " << student.getRollNo() << ",\n"
                 << "      \"name\": \"" << escapeJSON(student.getName()) << "\",\n"
                 << "      \"className\": \"" << escapeJSON(student.getClassName()) << "\"\n"
                 << "    }";
            if (i < students.size() - 1) file << ",";
            file << "\n";
        }

        file << "  ]\n}\n";
        file.close();
        Logger::getInstance().logAction("EXPORT_JSON", "Exported " + std::to_string(students.size()) + " records to " + filePath);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().logError("JSON export failed: " + std::string(e.what()));
        return false;
    }
}

bool Exporter::exportToTXT(const std::vector<Student>& students, const std::string& filePath) {
    try {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            Logger::getInstance().logError("Failed to open file for TXT export: " + filePath);
            return false;
        }

        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        file << "Student Management System - Export Report\n";
        file << "Generated: " << buffer << "\n";
        file << "Total Records: " << students.size() << "\n";
        file << std::string(60, '=') << "\n\n";
        file << std::left << std::setw(10) << "Roll No" << std::setw(30) << "Name" << std::setw(20) << "Class" << "\n";
        file << std::string(60, '-') << "\n";

        for (const auto& student : students) {
            file << std::left << std::setw(10) << student.getRollNo()
                 << std::setw(30) << student.getName()
                 << std::setw(20) << student.getClassName() << "\n";
        }

        file << std::string(60, '=') << "\n";
        file.close();
        Logger::getInstance().logAction("EXPORT_TXT", "Exported " + std::to_string(students.size()) + " records to " + filePath);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().logError("TXT export failed: " + std::string(e.what()));
        return false;
    }
}

std::string Exporter::escapeCSV(const std::string& field) {
    if (field.find(',') != std::string::npos || field.find('\"') != std::string::npos || field.find('\n') != std::string::npos) {
        std::string escaped = "\"";
        for (char c : field) {
            if (c == '\"') escaped += "\"\"";
            else escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return field;
}

std::string Exporter::escapeJSON(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '\"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\b': escaped += "\\b"; break;
            case '\f': escaped += "\\f"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += c;
        }
    }
    return escaped;
}
