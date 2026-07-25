#include "catch.hpp"
#include "student.h"

TEST_CASE("Student Creation", "[student]") {
    Student student(101, "John Doe", "Class A");
    
    REQUIRE(student.getRollNo() == 101);
    REQUIRE(student.getName() == "John Doe");
    REQUIRE(student.getClassName() == "Class A");
}

TEST_CASE("Student Modification", "[student]") {
    Student student(101, "John Doe", "Class A");
    
    student.setName("Jane Doe");
    student.setClassName("Class B");
    
    REQUIRE(student.getName() == "Jane Doe");
    REQUIRE(student.getClassName() == "Class B");
}

TEST_CASE("Student String Representation", "[student]") {
    Student student(101, "John Doe", "Class A");
    std::string str = student.toString();
    
    REQUIRE(str.find("101") != std::string::npos);
    REQUIRE(str.find("John Doe") != std::string::npos);
}
