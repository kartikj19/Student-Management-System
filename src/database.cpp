#include "database.h"
#include "logger.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>
#include <algorithm>

Database::Database(const std::string& dbPath) : db(nullptr), dbPath(dbPath) {}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        Logger::getInstance().logError("Failed to open database: " + dbPath);
        return false;
    }
    Logger::getInstance().logInfo("Database connected: " + dbPath);
    return createTables();
}

void Database::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        Logger::getInstance().logInfo("Database disconnected");
    }
}

bool Database::createTables() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS students (
            roll_no INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            class_name TEXT NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        Logger::getInstance().logError("Failed to create tables: " + std::string(errMsg ? errMsg : ""));
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::addStudent(const Student& student) {
    if (studentExists(student.getRollNo())) {
        Logger::getInstance().logError("Student with roll no " + std::to_string(student.getRollNo()) + " already exists");
        return false;
    }

    std::ostringstream sql;
    sql << "INSERT INTO students (roll_no, name, class_name) VALUES (" 
        << student.getRollNo() << ", '" 
        << student.getName() << "', '" 
        << student.getClassName() << "');";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        Logger::getInstance().logError("Failed to add student: " + std::string(errMsg ? errMsg : ""));
        sqlite3_free(errMsg);
        return false;
    }

    Logger::getInstance().logAction("ADD_STUDENT", "Roll No: " + std::to_string(student.getRollNo()) + ", Name: " + student.getName());
    return true;
}

bool Database::updateStudent(const Student& student) {
    std::ostringstream sql;
    sql << "UPDATE students SET name='" << student.getName() 
        << "', class_name='" << student.getClassName() 
        << "', updated_at=CURRENT_TIMESTAMP WHERE roll_no=" << student.getRollNo() << ";";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        Logger::getInstance().logError("Failed to update student: " + std::string(errMsg ? errMsg : ""));
        sqlite3_free(errMsg);
        return false;
    }

    Logger::getInstance().logAction("UPDATE_STUDENT", "Roll No: " + std::to_string(student.getRollNo()));
    return true;
}

bool Database::deleteStudent(int rollNo) {
    std::ostringstream sql;
    sql << "DELETE FROM students WHERE roll_no=" << rollNo << ";";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        Logger::getInstance().logError("Failed to delete student: " + std::string(errMsg ? errMsg : ""));
        sqlite3_free(errMsg);
        return false;
    }

    Logger::getInstance().logAction("DELETE_STUDENT", "Roll No: " + std::to_string(rollNo));
    return true;
}

Student* Database::getStudent(int rollNo) {
    std::ostringstream sql;
    sql << "SELECT roll_no, name, class_name FROM students WHERE roll_no=" << rollNo << ";";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    Student* student = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int roll = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string className = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        student = new Student(roll, name, className);
    }

    sqlite3_finalize(stmt);
    return student;
}

std::vector<Student> Database::getAllStudents() {
    return executeQuery("SELECT roll_no, name, class_name FROM students ORDER BY roll_no;");
}

std::vector<Student> Database::searchByName(const std::string& name) {
    std::ostringstream sql;
    sql << "SELECT roll_no, name, class_name FROM students WHERE name LIKE '%" << name << "%';";
    return executeQuery(sql.str());
}

std::vector<Student> Database::searchByClass(const std::string& className) {
    std::ostringstream sql;
    sql << "SELECT roll_no, name, class_name FROM students WHERE class_name = '" << className << "';";
    return executeQuery(sql.str());
}

bool Database::studentExists(int rollNo) {
    std::ostringstream sql;
    sql << "SELECT 1 FROM students WHERE roll_no=" << rollNo << " LIMIT 1;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    bool exists = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return exists;
}

int Database::getTotalStudents() {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM students;", -1, &stmt, nullptr) != SQLITE_OK) {
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

std::vector<std::pair<std::string, int>> Database::getStudentsPerClass() {
    std::vector<std::pair<std::string, int>> result;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, "SELECT class_name, COUNT(*) FROM students GROUP BY class_name ORDER BY class_name;", -1, &stmt, nullptr) != SQLITE_OK) {
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string className = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int count = sqlite3_column_int(stmt, 1);
        result.push_back({className, count});
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<Student> Database::getSortedByName(bool ascending) {
    std::string order = ascending ? "ASC" : "DESC";
    std::ostringstream sql;
    sql << "SELECT roll_no, name, class_name FROM students ORDER BY name " << order << ";";
    return executeQuery(sql.str());
}

std::vector<Student> Database::getSortedByRollNo(bool ascending) {
    std::string order = ascending ? "ASC" : "DESC";
    std::ostringstream sql;
    sql << "SELECT roll_no, name, class_name FROM students ORDER BY roll_no " << order << ";";
    return executeQuery(sql.str());
}

std::vector<Student> Database::getSortedByClass(bool ascending) {
    std::string order = ascending ? "ASC" : "DESC";
    std::ostringstream sql;
    sql << "SELECT roll_no, name, class_name FROM students ORDER BY class_name " << order << ";";
    return executeQuery(sql.str());
}

bool Database::backup(const std::string& backupPath) {
    try {
        std::ifstream source(dbPath, std::ios::binary);
        std::ofstream dest(backupPath, std::ios::binary);
        
        if (!source || !dest) {
            Logger::getInstance().logError("Failed to backup database");
            return false;
        }
        
        dest << source.rdbuf();
        source.close();
        dest.close();
        
        Logger::getInstance().logAction("BACKUP", "Backup created at " + backupPath);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().logError("Backup failed: " + std::string(e.what()));
        return false;
    }
}

bool Database::restore(const std::string& backupPath) {
    try {
        std::ifstream source(backupPath, std::ios::binary);
        std::ofstream dest(dbPath, std::ios::binary);
        
        if (!source || !dest) {
            Logger::getInstance().logError("Failed to restore database");
            return false;
        }
        
        dest << source.rdbuf();
        source.close();
        dest.close();
        
        Logger::getInstance().logAction("RESTORE", "Database restored from " + backupPath);
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().logError("Restore failed: " + std::string(e.what()));
        return false;
    }
}

int Database::getMaxRollNo() {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT MAX(roll_no) FROM students;", -1, &stmt, nullptr) != SQLITE_OK) {
        return 0;
    }

    int maxRoll = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
        maxRoll = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return maxRoll;
}

double Database::getAverageNameLength() {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT AVG(LENGTH(name)) FROM students;", -1, &stmt, nullptr) != SQLITE_OK) {
        return 0.0;
    }

    double avg = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
        avg = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return avg;
}

std::vector<Student> Database::executeQuery(const std::string& query) {
    std::vector<Student> students;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return students;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int roll = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string className = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        students.emplace_back(roll, name, className);
    }

    sqlite3_finalize(stmt);
    return students;
}

int Database::queryCallback(void* data, int argc, char** argv, char** azColName) {
    return 0;
}
