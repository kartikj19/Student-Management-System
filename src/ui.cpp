#include "ui.h"
#include "exporter.h"
#include "analytics.h"
#include "logger.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

UI::UI(std::shared_ptr<Database> db, std::shared_ptr<Validator> val)
    : database(db), validator(val) {}

void UI::run() {
    int choice = 0;
    while (true) {
        clearScreen();
        displayMainMenu();
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudent();
                break;
            case 3:
                modifyStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                displayAllStudents();
                break;
            case 6:
                displaySearchMenu();
                break;
            case 7:
                sortAndDisplay();
                break;
            case 8:
                displayExportMenu();
                break;
            case 9:
                displayReportsMenu();
                break;
            case 10:
                displaySettingsMenu();
                break;
            case 11:
                Logger::getInstance().logInfo("Application closed by user");
                clearScreen();
                std::cout << "\nThank you for using Student Management System!\n\n";
                return;
            default:
                std::cout << "\nInvalid choice! Please try again.\n";
                pauseScreen();
        }
    }
}

void UI::displayMainMenu() {
    displayHeader("STUDENT MANAGEMENT SYSTEM");
    std::cout << "\n\n01. Add Student\n";
    std::cout << "02. Search Student\n";
    std::cout << "03. Modify Student\n";
    std::cout << "04. Delete Student\n";
    std::cout << "05. Display All Students\n";
    std::cout << "06. Search Menu\n";
    std::cout << "07. Sort Students\n";
    std::cout << "08. Export Data\n";
    std::cout << "09. Reports & Analytics\n";
    std::cout << "10. Settings\n";
    std::cout << "11. Exit\n";
    displayFooter();
}

void UI::displayDataMenu() {
    std::cout << "\n01. Add Student\n";
    std::cout << "02. View Student\n";
    std::cout << "03. Modify Student\n";
    std::cout << "04. Delete Student\n";
    std::cout << "05. Back to Main Menu\n";
}

void UI::displaySearchMenu() {
    clearScreen();
    displayHeader("SEARCH MENU");
    
    int choice = 0;
    while (choice != 3) {
        std::cout << "\n01. Search by Name\n";
        std::cout << "02. Search by Class\n";
        std::cout << "03. Back to Main Menu\n";
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1:
                searchByName();
                break;
            case 2:
                searchByClass();
                break;
            case 3:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void UI::displayExportMenu() {
    clearScreen();
    displayHeader("EXPORT DATA");
    
    int choice = 0;
    while (choice != 4) {
        std::cout << "\n01. Export to CSV\n";
        std::cout << "02. Export to JSON\n";
        std::cout << "03. Export to Text\n";
        std::cout << "04. Back to Main Menu\n";
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1:
                exportToCSV();
                break;
            case 2:
                exportToJSON();
                break;
            case 3: {
                auto students = database->getAllStudents();
                std::string path = getInput("Enter file path (e.g., students.txt): ");
                if (Exporter::exportToTXT(students, path)) {
                    std::cout << "\nExport successful!\n";
                } else {
                    std::cout << "\nExport failed!\n";
                }
                pauseScreen();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void UI::displayReportsMenu() {
    clearScreen();
    displayHeader("REPORTS & ANALYTICS");
    
    int choice = 0;
    Analytics analytics(database);
    
    while (choice != 5) {
        std::cout << "\n01. Dashboard\n";
        std::cout << "02. Class Distribution Report\n";
        std::cout << "03. Student Count Report\n";
        std::cout << "04. Detailed Statistics\n";
        std::cout << "05. Back to Main Menu\n";
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1:
                clearScreen();
                analytics.displayDashboard();
                pauseScreen();
                clearScreen();
                displayHeader("REPORTS & ANALYTICS");
                break;
            case 2:
                clearScreen();
                analytics.generateClassReport();
                pauseScreen();
                clearScreen();
                displayHeader("REPORTS & ANALYTICS");
                break;
            case 3:
                clearScreen();
                analytics.generateStudentCountReport();
                pauseScreen();
                clearScreen();
                displayHeader("REPORTS & ANALYTICS");
                break;
            case 4:
                clearScreen();
                analytics.generateDetailedReport();
                pauseScreen();
                clearScreen();
                displayHeader("REPORTS & ANALYTICS");
                break;
            case 5:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void UI::displaySettingsMenu() {
    clearScreen();
    displayHeader("SETTINGS");
    
    int choice = 0;
    while (choice != 3) {
        std::cout << "\n01. Backup Database\n";
        std::cout << "02. Restore Database\n";
        std::cout << "03. Back to Main Menu\n";
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1:
                backupData();
                break;
            case 2:
                restoreData();
                break;
            case 3:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void UI::addStudent() {
    clearScreen();
    displayHeader("ADD STUDENT");

    int rollNo = getIntInput("Enter Roll No: ");
    if (!validator->validateRollNo(rollNo)) {
        std::cout << "Error: " << validator->getLastError() << "\n";
        pauseScreen();
        return;
    }

    if (database->studentExists(rollNo)) {
        std::cout << "Error: Student with this roll number already exists!\n";
        pauseScreen();
        return;
    }

    std::string name = getInput("Enter Name: ");
    if (!validator->validateName(name)) {
        std::cout << "Error: " << validator->getLastError() << "\n";
        pauseScreen();
        return;
    }

    std::string className = getInput("Enter Class: ");
    if (!validator->validateClassName(className)) {
        std::cout << "Error: " << validator->getLastError() << "\n";
        pauseScreen();
        return;
    }

    Student student(rollNo, name, className);
    if (database->addStudent(student)) {
        std::cout << "\nStudent added successfully!\n";
    } else {
        std::cout << "\nFailed to add student!\n";
    }
    pauseScreen();
}

void UI::displayStudent() {
    clearScreen();
    displayHeader("SEARCH STUDENT");

    int rollNo = getIntInput("Enter Roll No to search: ");
    auto student = database->getStudent(rollNo);

    if (student) {
        std::cout << "\n" << std::string(50, '-') << "\n";
        std::cout << "Roll No: " << student->getRollNo() << "\n";
        std::cout << "Name: " << student->getName() << "\n";
        std::cout << "Class: " << student->getClassName() << "\n";
        std::cout << std::string(50, '-') << "\n";
        delete student;
    } else {
        std::cout << "\nStudent not found!\n";
    }
    pauseScreen();
}

void UI::modifyStudent() {
    clearScreen();
    displayHeader("MODIFY STUDENT");

    int rollNo = getIntInput("Enter Roll No to modify: ");
    auto student = database->getStudent(rollNo);

    if (!student) {
        std::cout << "\nStudent not found!\n";
        pauseScreen();
        return;
    }

    std::cout << "\nCurrent Information:\n";
    std::cout << "Roll No: " << student->getRollNo() << "\n";
    std::cout << "Name: " << student->getName() << "\n";
    std::cout << "Class: " << student->getClassName() << "\n\n";

    std::string name = getInput("Enter new Name (or press Enter to keep): ");
    if (!name.empty()) {
        if (!validator->validateName(name)) {
            std::cout << "Error: " << validator->getLastError() << "\n";
            delete student;
            pauseScreen();
            return;
        }
        student->setName(name);
    }

    std::string className = getInput("Enter new Class (or press Enter to keep): ");
    if (!className.empty()) {
        if (!validator->validateClassName(className)) {
            std::cout << "Error: " << validator->getLastError() << "\n";
            delete student;
            pauseScreen();
            return;
        }
        student->setClassName(className);
    }

    student->setUpdatedAt();
    if (database->updateStudent(*student)) {
        std::cout << "\nStudent updated successfully!\n";
    } else {
        std::cout << "\nFailed to update student!\n";
    }
    delete student;
    pauseScreen();
}

void UI::deleteStudent() {
    clearScreen();
    displayHeader("DELETE STUDENT");

    int rollNo = getIntInput("Enter Roll No to delete: ");
    
    char confirm;
    std::cout << "Are you sure? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        if (database->deleteStudent(rollNo)) {
            std::cout << "\nStudent deleted successfully!\n";
        } else {
            std::cout << "\nFailed to delete student!\n";
        }
    } else {
        std::cout << "\nDeletion cancelled!\n";
    }
    pauseScreen();
}

void UI::displayAllStudents() {
    clearScreen();
    displayHeader("ALL STUDENTS");

    auto students = database->getAllStudents();

    if (students.empty()) {
        std::cout << "\nNo students found!\n";
    } else {
        std::cout << "\n" << std::left << std::setw(12) << "Roll No" << std::setw(30) << "Name" << std::setw(20) << "Class" << "\n";
        std::cout << std::string(62, '-') << "\n";
        for (const auto& student : students) {
            std::cout << std::left << std::setw(12) << student.getRollNo()
                      << std::setw(30) << student.getName()
                      << std::setw(20) << student.getClassName() << "\n";
        }
        std::cout << std::string(62, '-') << "\n";
        std::cout << "Total: " << students.size() << " student(s)\n";
    }
    pauseScreen();
}

void UI::searchByName() {
    clearScreen();
    displayHeader("SEARCH BY NAME");

    std::string name = getInput("Enter name to search: ");
    auto students = database->searchByName(name);

    if (students.empty()) {
        std::cout << "\nNo students found with name containing: " << name << "\n";
    } else {
        std::cout << "\n" << std::left << std::setw(12) << "Roll No" << std::setw(30) << "Name" << std::setw(20) << "Class" << "\n";
        std::cout << std::string(62, '-') << "\n";
        for (const auto& student : students) {
            std::cout << std::left << std::setw(12) << student.getRollNo()
                      << std::setw(30) << student.getName()
                      << std::setw(20) << student.getClassName() << "\n";
        }
        std::cout << std::string(62, '-') << "\n";
        std::cout << "Found: " << students.size() << " student(s)\n";
    }
    pauseScreen();
}

void UI::searchByClass() {
    clearScreen();
    displayHeader("SEARCH BY CLASS");

    std::string className = getInput("Enter class to search: ");
    auto students = database->searchByClass(className);

    if (students.empty()) {
        std::cout << "\nNo students found in class: " << className << "\n";
    } else {
        std::cout << "\n" << std::left << std::setw(12) << "Roll No" << std::setw(30) << "Name" << std::setw(20) << "Class" << "\n";
        std::cout << std::string(62, '-') << "\n";
        for (const auto& student : students) {
            std::cout << std::left << std::setw(12) << student.getRollNo()
                      << std::setw(30) << student.getName()
                      << std::setw(20) << student.getClassName() << "\n";
        }
        std::cout << std::string(62, '-') << "\n";
        std::cout << "Found: " << students.size() << " student(s)\n";
    }
    pauseScreen();
}

void UI::sortAndDisplay() {
    clearScreen();
    displayHeader("SORT STUDENTS");

    int choice = getIntInput("Sort by:\n01. Roll No (Ascending)\n02. Roll No (Descending)\n03. Name (Ascending)\n04. Name (Descending)\n05. Class (Ascending)\n06. Class (Descending)\nEnter choice: ");

    std::vector<Student> students;

    switch (choice) {
        case 1:
            students = database->getSortedByRollNo(true);
            break;
        case 2:
            students = database->getSortedByRollNo(false);
            break;
        case 3:
            students = database->getSortedByName(true);
            break;
        case 4:
            students = database->getSortedByName(false);
            break;
        case 5:
            students = database->getSortedByClass(true);
            break;
        case 6:
            students = database->getSortedByClass(false);
            break;
        default:
            std::cout << "Invalid choice!\n";
            pauseScreen();
            return;
    }

    clearScreen();
    displayHeader("SORTED STUDENTS");

    if (students.empty()) {
        std::cout << "\nNo students found!\n";
    } else {
        std::cout << "\n" << std::left << std::setw(12) << "Roll No" << std::setw(30) << "Name" << std::setw(20) << "Class" << "\n";
        std::cout << std::string(62, '-') << "\n";
        for (const auto& student : students) {
            std::cout << std::left << std::setw(12) << student.getRollNo()
                      << std::setw(30) << student.getName()
                      << std::setw(20) << student.getClassName() << "\n";
        }
        std::cout << std::string(62, '-') << "\n";
        std::cout << "Total: " << students.size() << " student(s)\n";
    }
    pauseScreen();
}

void UI::exportToCSV() {
    clearScreen();
    displayHeader("EXPORT TO CSV");

    auto students = database->getAllStudents();
    if (students.empty()) {
        std::cout << "\nNo students to export!\n";
        pauseScreen();
        return;
    }

    std::string filePath = getInput("Enter file path (e.g., students.csv): ");
    if (Exporter::exportToCSV(students, filePath)) {
        std::cout << "\nExport successful! File saved to: " << filePath << "\n";
    } else {
        std::cout << "\nExport failed!\n";
    }
    pauseScreen();
}

void UI::exportToJSON() {
    clearScreen();
    displayHeader("EXPORT TO JSON");

    auto students = database->getAllStudents();
    if (students.empty()) {
        std::cout << "\nNo students to export!\n";
        pauseScreen();
        return;
    }

    std::string filePath = getInput("Enter file path (e.g., students.json): ");
    if (Exporter::exportToJSON(students, filePath)) {
        std::cout << "\nExport successful! File saved to: " << filePath << "\n";
    } else {
        std::cout << "\nExport failed!\n";
    }
    pauseScreen();
}

void UI::viewStatistics() {
    clearScreen();
    displayHeader("STATISTICS");
    
    int total = database->getTotalStudents();
    std::cout << "\nTotal Students: " << total << "\n";
    pauseScreen();
}

void UI::viewAuditLog() {
    clearScreen();
    displayHeader("AUDIT LOG");
    std::cout << "\nView logs/audit.log file for detailed audit trail\n";
    pauseScreen();
}

void UI::backupData() {
    clearScreen();
    displayHeader("BACKUP DATABASE");

    std::string backupPath = getInput("Enter backup file path (e.g., backup_students.db): ");
    if (database->backup(backupPath)) {
        std::cout << "\nBackup successful! File saved to: " << backupPath << "\n";
    } else {
        std::cout << "\nBackup failed!\n";
    }
    pauseScreen();
}

void UI::restoreData() {
    clearScreen();
    displayHeader("RESTORE DATABASE");

    std::string backupPath = getInput("Enter backup file path to restore from: ");
    
    char confirm;
    std::cout << "Warning: This will overwrite current data. Continue? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        if (database->restore(backupPath)) {
            std::cout << "\nRestore successful!\n";
        } else {
            std::cout << "\nRestore failed!\n";
        }
    } else {
        std::cout << "\nRestore cancelled!\n";
    }
    pauseScreen();
}

void UI::clearScreen() const {
    system(CLEAR_SCREEN);
}

void UI::pauseScreen() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UI::displayHeader(const std::string& title) const {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << std::setw(60 - 1) << std::right << "\n";
    int padding = (60 - title.length()) / 2;
    std::cout << std::string(padding, ' ') << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void UI::displayFooter() const {
    std::cout << "\n" << std::string(60, '-') << "\n";
}

std::string UI::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return validator->trim(input);
}

int UI::getIntInput(const std::string& prompt) {
    std::cout << prompt;
    int input;
    while (!(std::cin >> input)) {
        std::cout << "Invalid input. Please enter a valid number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}
