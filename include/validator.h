#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <vector>

class Validator {
public:
    Validator();

    // Validation methods
    bool validateRollNo(int rollNo);
    bool validateName(const std::string& name);
    bool validateClassName(const std::string& className);
    bool validateEmail(const std::string& email);
    bool validatePhone(const std::string& phone);
    bool validateFilePath(const std::string& path);

    // Utility methods
    std::string trim(const std::string& str);
    std::string toLower(const std::string& str);
    bool isNumeric(const std::string& str);
    bool isAlphabetic(const std::string& str);
    bool isAlphanumeric(const std::string& str);

    // Getters for error messages
    const std::string& getLastError() const { return lastError; }

private:
    std::string lastError;
    void setError(const std::string& error) { lastError = error; }
};

#endif // VALIDATOR_H
