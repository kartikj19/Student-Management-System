cmake_minimum_required(VERSION 3.16)
project(StudentManagementSystem VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Try to find system SQLite3, but don't fail if not found
find_package(SQLite3 QUIET)

# If SQLite3 not found, use FetchContent to download bundled version
if(NOT SQLite3_FOUND)
    message(STATUS "System SQLite3 not found. Downloading bundled SQLite3...")
    include(FetchContent)
    
    FetchContent_Declare(
        sqlite_amalgamation
        URL https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        QUIET
    )
    
    FetchContent_MakeAvailable(sqlite_amalgamation)
    
    # Create a library from the SQLite3 amalgamation
    add_library(sqlite3_bundled STATIC 
        "${sqlite_amalgamation_SOURCE_DIR}/sqlite3.c"
    )
    
    target_include_directories(sqlite3_bundled PUBLIC
        "${sqlite_amalgamation_SOURCE_DIR}"
    )
    
    # Suppress warnings from SQLite3 source
    if(MSVC)
        target_compile_options(sqlite3_bundled PRIVATE /W0)
    else()
        target_compile_options(sqlite3_bundled PRIVATE -w)
    endif()
    
    # Create an alias for consistency
    add_library(SQLite::SQLite3 ALIAS sqlite3_bundled)
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
