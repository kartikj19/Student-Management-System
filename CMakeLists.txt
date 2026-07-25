cmake_minimum_required(VERSION 3.14)
project(StudentManagementSystem VERSION 2.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# =============================================================================
# SQLite3 - Download and build from source
# =============================================================================
message(STATUS "Setting up SQLite3 from source...")

set(SQLITE3_SOURCE_DIR "${CMAKE_BINARY_DIR}/sqlite3_source")
set(SQLITE3_ZIP "${CMAKE_BINARY_DIR}/sqlite3.zip")

# Download SQLite3 if not already present
if(NOT EXISTS "${SQLITE3_SOURCE_DIR}/sqlite3.c")
    message(STATUS "Downloading SQLite3 source code...")
    
    file(DOWNLOAD 
        "https://www.sqlite.org/2024/sqlite-amalgamation-3460100.zip"
        "${SQLITE3_ZIP}"
        SHOW_PROGRESS
        STATUS DOWNLOAD_STATUS
    )
    
    list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
    if(NOT DOWNLOAD_CODE EQUAL 0)
        message(FATAL_ERROR "Failed to download SQLite3. Check your internet connection.")
    endif()
    
    # Extract the zip file
    message(STATUS "Extracting SQLite3 source...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf "${SQLITE3_ZIP}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )
    
    if(NOT TAR_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to extract SQLite3 source")
    endif()
    
    # Move extracted files to expected location
    file(RENAME 
        "${CMAKE_BINARY_DIR}/sqlite-amalgamation-3460100"
        "${SQLITE3_SOURCE_DIR}"
    )
endif()

message(STATUS "SQLite3 source ready at ${SQLITE3_SOURCE_DIR}")

# Create SQLite3 static library
add_library(sqlite3 STATIC "${SQLITE3_SOURCE_DIR}/sqlite3.c")

target_include_directories(sqlite3 PUBLIC 
    "${SQLITE3_SOURCE_DIR}"
)

set_target_properties(sqlite3 PROPERTIES LANGUAGE C)

# Suppress warnings from SQLite3 source
if(MSVC)
    target_compile_options(sqlite3 PRIVATE /W0)
else()
    target_compile_options(sqlite3 PRIVATE -w)
endif()

# =============================================================================
# Main application
# =============================================================================
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
    "${SQLITE3_SOURCE_DIR}"
)

target_link_libraries(student_management_system PRIVATE
    sqlite3
)

# =============================================================================
# Tests
# =============================================================================
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
    "${SQLITE3_SOURCE_DIR}"
)

target_link_libraries(tests PRIVATE
    sqlite3
)

add_test(NAME AllTests COMMAND tests)

message(STATUS "Build configured successfully!")
message(STATUS "Run 'mingw32-make' or 'make' to build")
