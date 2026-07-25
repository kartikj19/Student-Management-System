cmake_minimum_required(VERSION 3.14)
project(StudentManagementSystem VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# SQLite3 handling - try system first, then embedded
find_package(SQLite3 QUIET)

if(NOT SQLite3_FOUND)
    message(STATUS "System SQLite3 not found. Using embedded SQLite3 source.")
    
    # Create sqlite3 library from source
    # Using the built-in SQLite amalgamation approach
    file(WRITE "${CMAKE_BINARY_DIR}/sqlite3_config.h" "
#define SQLITE_THREADSAFE 1
#define SQLITE_ENABLE_FTS3 1
#define SQLITE_ENABLE_JSON1 1
")
    
    # Download SQLite source if not present
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/sqlite3.c")
        message(STATUS "Downloading SQLite3 source...")
        file(DOWNLOAD
            "https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip"
            "${CMAKE_BINARY_DIR}/sqlite3.zip"
            SHOW_PROGRESS
        )
        
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_BINARY_DIR}/sqlite3.zip"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
        
        file(COPY "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3460100/sqlite3.c" 
             DESTINATION "${CMAKE_BINARY_DIR}")
        file(COPY "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3460100/sqlite3.h" 
             DESTINATION "${CMAKE_BINARY_DIR}")
    endif()
    
    add_library(sqlite3 STATIC "${CMAKE_BINARY_DIR}/sqlite3.c")
    target_include_directories(sqlite3 PUBLIC "${CMAKE_BINARY_DIR}")
    
    if(MSVC)
        target_compile_options(sqlite3 PRIVATE /W0)
    else()
        target_compile_options(sqlite3 PRIVATE -w)
    endif()
    
    add_library(SQLite::SQLite3 ALIAS sqlite3)
else()
    message(STATUS "Using system SQLite3")
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

# Enable testing
enable_testing()

# Tests executable
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
