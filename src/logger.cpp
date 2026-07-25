#include "logger.h"
#include <iostream>
#include <iomanip>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {}

void Logger::init(const std::string& logPath) {
    logFile.open(logPath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Warning: Could not open log file at " << logPath << std::endl;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::string timestamp = getTimestamp();
    std::string levelStr = getLevelString(level);
    std::string logEntry = "[" + timestamp + "] [" + levelStr + "] " + message;

    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }

    if (level == LogLevel::ERROR || level == LogLevel::CRITICAL) {
        std::cerr << logEntry << std::endl;
    }
}

void Logger::logAction(const std::string& action, const std::string& details) {
    log(LogLevel::INFO, "ACTION: " + action + " - " + details);
}

void Logger::logError(const std::string& error) {
    log(LogLevel::ERROR, error);
}

void Logger::logInfo(const std::string& info) {
    log(LogLevel::INFO, info);
}

std::string Logger::getTimestamp() const {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}
