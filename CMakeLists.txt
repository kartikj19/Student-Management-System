cmake_minimum_required(VERSION 3.16)
project(StudentManagementSystem VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# SQLite dependency resolution:
# 1) Prefer system SQLite3 if available
# 2) Otherwise use bundled SQLite amalgamation under third_party/sqlite
find_package(SQLite3 QUIET)

if(NOT SQLite3_FOUND)
    set(BUNDLED_SQLITE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sqlite)
    set(BUNDLED_SQLITE_SRC ${BUNDLED_SQLITE_DIR}/sqlite3.c)

    if(NOT EXISTS ${BUNDLED_SQLITE_SRC})
        message(FATAL_ERROR
            "System SQLite3 was not found and bundled SQLite source is missing at ${BUNDLED_SQLITE_SRC}."
        )
    endif()

    message(STATUS "System SQLite3 not found. Falling back to bundled SQLite source.")

    add_library(sqlite3_amalgamation STATIC
        ${BUNDLED_SQLITE_SRC}
    )
    target_include_directories(sqlite3_amalgamation PUBLIC
        ${BUNDLED_SQLITE_DIR}
    )

    add_library(SQLite::SQLite3 ALIAS sqlite3_amalgamation)
endif()

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
