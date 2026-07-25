#include "catch.hpp"
#include "database.h"
#include <fstream>
#include <cstdio>

class TestDatabase {
public:
    static void cleanup() {
        std::remove("test_students.db");
    }
};

TEST_CASE("Database Connection", "[database]") {
    TestDatabase::cleanup();
    Database db("test_students.db");
    
    REQUIRE(db.connect() == true);
    REQUIRE(db.isConnected() == true);
    
    db.disconnect();
    TestDatabase::cleanup();
}

TEST_CASE("Add and Retrieve Student", "[database]") {
    TestDatabase::cleanup();
    Database db("test_students.db");
    db.connect();
    
    Student student(101, "John Doe", "Class A");
    REQUIRE(db.addStudent(student) == true);
    
    auto retrieved = db.getStudent(101);
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getRollNo() == 101);
    REQUIRE(retrieved->getName() == "John Doe");
    
    delete retrieved;
    db.disconnect();
    TestDatabase::cleanup();
}

TEST_CASE("Get All Students", "[database]") {
    TestDatabase::cleanup();
    Database db("test_students.db");
    db.connect();
    
    db.addStudent(Student(101, "John Doe", "Class A"));
    db.addStudent(Student(102, "Jane Doe", "Class B"));
    
    auto students = db.getAllStudents();
    REQUIRE(students.size() == 2);
    
    db.disconnect();
    TestDatabase::cleanup();
}
