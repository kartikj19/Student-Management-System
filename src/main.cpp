#include "ui.h"
#include "database.h"
#include "validator.h"
#include "logger.h"
#include <memory>
#include <iostream>

int main() {
    try {
        // Initialize logger
        Logger::getInstance().init("logs/audit.log");
        Logger::getInstance().logInfo("Application started");

        // Create database connection
        auto database = std::make_shared<Database>("students.db");
        if (!database->connect()) {
            std::cerr << "Error: Failed to connect to database" << std::endl;
            Logger::getInstance().logError("Failed to connect to database");
            return 1;
        }

        // Create validator
        auto validator = std::make_shared<Validator>();

        // Create and run UI
        UI ui(database, validator);
        ui.run();

        // Cleanup
        database->disconnect();
        Logger::getInstance().logInfo("Application closed");

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        Logger::getInstance().logError(std::string("Fatal error: ") + e.what());
        return 1;
    }
}
