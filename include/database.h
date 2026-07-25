#ifndef DATABASE_H
#define DATABASE_H

#include "student.h"
#include <vector>
#include <memory>
#include <sqlite3.h>
#include <functional>

class Database {
public:
    Database(const std::string& dbPath = "students.db");
    ~Database();

    // Connection management
    bool connect();
    void disconnect();
    bool isConnected() const { return db != nullptr; }

    // CRUD operations
    bool addStudent(const Student& student);
    bool updateStudent(const Student& student);
    bool deleteStudent(int rollNo);
    Student* getStudent(int rollNo);
    std::vector<Student> getAllStudents();

    // Query operations
    std::vector<Student> searchByName(const std::string& name);
    std::vector<Student> searchByClass(const std::string& className);
    bool studentExists(int rollNo);
    int getTotalStudents();
    std::vector<std::pair<std::string, int>> getStudentsPerClass();

    // Sorting
    std::vector<Student> getSortedByName(bool ascending = true);
    std::vector<Student> getSortedByRollNo(bool ascending = true);
    std::vector<Student> getSortedByClass(bool ascending = true);

    // Backup and restore
    bool backup(const std::string& backupPath);
    bool restore(const std::string& backupPath);

    // Statistics
    int getMaxRollNo();
    double getAverageNameLength();

private:
    sqlite3* db;
    std::string dbPath;
    bool createTables();
    std::vector<Student> executeQuery(const std::string& query);
    static int queryCallback(void* data, int argc, char** argv, char** azColName);
};

#endif // DATABASE_H
