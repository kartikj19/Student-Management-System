cmake_minimum_required(VERSION 3.16)
project(StudentManagementSystem VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Find required packages
find_package(SQLite3 REQUIRED)

# Main executable
add_executable(student_management_system
    src/main.cpp
    src/database.cpp
    src/student.cpp
    src/ui.cpp
    src/validator.cpp
    src/logger.cpp
    src/exporter.cpp
    src/analytics.cpp
)

target_include_directories(student_management_system PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(student_management_system PRIVATE
    SQLite::SQLite3
)

# Tests
enable_testing()
add_executable(tests
    tests/test_main.cpp
    tests/test_student.cpp
    tests/test_validator.cpp
    tests/test_database.cpp
    src/student.cpp
    src/validator.cpp
    src/database.cpp
    src/logger.cpp
    src/exporter.cpp
    src/analytics.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE
    SQLite::SQLite3
)

add_test(NAME AllTests COMMAND tests)
