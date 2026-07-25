#ifndef UI_H
#define UI_H

#include "database.h"
#include "validator.h"
#include <string>
#include <memory>

class UI {
public:
    UI(std::shared_ptr<Database> db, std::shared_ptr<Validator> validator);
    void run();

private:
    std::shared_ptr<Database> database;
    std::shared_ptr<Validator> validator;

    // Menu functions
    void displayMainMenu();
    void displayDataMenu();
    void displaySearchMenu();
    void displayExportMenu();
    void displayReportsMenu();
    void displaySettingsMenu();

    // Operations
    void addStudent();
    void displayStudent();
    void modifyStudent();
    void deleteStudent();
    void displayAllStudents();
    void searchByName();
    void searchByClass();
    void sortAndDisplay();
    void exportToCSV();
    void exportToJSON();
    void viewStatistics();
    void viewAuditLog();
    void backupData();
    void restoreData();
    void clearScreen() const;
    void pauseScreen() const;
    void displayHeader(const std::string& title) const;
    void displayFooter() const;
    std::string getInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);
};

#endif // UI_H
