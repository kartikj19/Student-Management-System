#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <ctime>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
public:
    static Logger& getInstance();
    void init(const std::string& logPath = "logs/audit.log");
    void log(LogLevel level, const std::string& message);
    void logAction(const std::string& action, const std::string& details);
    void logError(const std::string& error);
    void logInfo(const std::string& info);

private:
    Logger();
    std::ofstream logFile;
    std::string getTimestamp() const;
    std::string getLevelString(LogLevel level) const;
};

#endif // LOGGER_H
