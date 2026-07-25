#include "validator.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <fstream>

Validator::Validator() : lastError("") {}

bool Validator::validateRollNo(int rollNo) {
    if (rollNo <= 0) {
        setError("Roll number must be positive");
        return false;
    }
    if (rollNo > 999999) {
        setError("Roll number is too large");
        return false;
    }
    return true;
}

bool Validator::validateName(const std::string& name) {
    std::string trimmed = trim(name);
    if (trimmed.empty()) {
        setError("Name cannot be empty");
        return false;
    }
    if (trimmed.length() > 100) {
        setError("Name is too long (max 100 characters)");
        return false;
    }
    if (!std::all_of(trimmed.begin(), trimmed.end(), [](unsigned char c) {
        return std::isalpha(c) || std::isspace(c) || c == '-' || c == '.';
    })) {
        setError("Name contains invalid characters");
        return false;
    }
    return true;
}

bool Validator::validateClassName(const std::string& className) {
    std::string trimmed = trim(className);
    if (trimmed.empty()) {
        setError("Class name cannot be empty");
        return false;
    }
    if (trimmed.length() > 50) {
        setError("Class name is too long (max 50 characters)");
        return false;
    }
    return true;
}

bool Validator::validateEmail(const std::string& email) {
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!std::regex_match(email, emailRegex)) {
        setError("Invalid email format");
        return false;
    }
    return true;
}

bool Validator::validatePhone(const std::string& phone) {
    if (!isNumeric(phone) || phone.length() < 10 || phone.length() > 15) {
        setError("Invalid phone number");
        return false;
    }
    return true;
}

bool Validator::validateFilePath(const std::string& path) {
    if (path.empty()) {
        setError("File path cannot be empty");
        return false;
    }
    return true;
}

std::string Validator::trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

std::string Validator::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool Validator::isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(),
                                       [](unsigned char c) { return std::isdigit(c); });
}

bool Validator::isAlphabetic(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(),
                                       [](unsigned char c) { return std::isalpha(c); });
}

bool Validator::isAlphanumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(),
                                       [](unsigned char c) { return std::isalnum(c); });
}
