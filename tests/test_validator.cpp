#include "catch.hpp"
#include "validator.h"

TEST_CASE("Validate Roll No", "[validator]") {
    Validator validator;
    
    REQUIRE(validator.validateRollNo(101) == true);
    REQUIRE(validator.validateRollNo(0) == false);
    REQUIRE(validator.validateRollNo(-1) == false);
}

TEST_CASE("Validate Name", "[validator]") {
    Validator validator;
    
    REQUIRE(validator.validateName("John Doe") == true);
    REQUIRE(validator.validateName("") == false);
    REQUIRE(validator.validateName("John123") == false);
}

TEST_CASE("Validate Class Name", "[validator]") {
    Validator validator;
    
    REQUIRE(validator.validateClassName("Class A") == true);
    REQUIRE(validator.validateClassName("") == false);
}

TEST_CASE("String Trim", "[validator]") {
    Validator validator;
    
    REQUIRE(validator.trim("  hello  ") == "hello");
    REQUIRE(validator.trim("world") == "world");
}

TEST_CASE("Is Numeric", "[validator]") {
    Validator validator;
    
    REQUIRE(validator.isNumeric("123") == true);
    REQUIRE(validator.isNumeric("12a") == false);
}
