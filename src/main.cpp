// ============================================================
//  SMART EXPENSE TRACKER
//  A console-based personal finance manager written in C++17.
//
//  This file only handles the menu / user interaction. All the
//  real logic lives in ExpenseManager (data) and Report (output),
//  so this file stays short and easy to follow.
// ============================================================

#include <iostream>
#include <limits>
#include <string>
#include "Expense.h"
#include "ExpenseManager.h"
#include "Report.h"

// -------------------------------------------------------------
// Input-validation helpers
//
// These functions keep asking the user until they give valid
// input, so the rest of the program never has to worry about
// garbage data. This is what stops the program from crashing
// on bad input (Feature 10).
// -------------------------------------------------------------

// Clears a failed cin state and throws away the rest of the bad line.
static void clearInputError() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Reads a whole line of text, re-prompting until it is non-empty.
static std::string readNonEmptyLine(const std::string& prompt) {
    std::string line;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, line);
        if (!line.empty()) {
            return line;
        }
        std::cout << "This field cannot be empty. Please try again.\n";
    }
}

// Reads a double, re-prompting until the input is a valid, non-negative number.

// Reads a real calendar date in strict YYYY-MM-DD format.
static std::string readValidDate(const std::string& prompt) {
    std::string date;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, date);

        if (isValidDate(date)) {
            return date;
        }

        std::cout << "Invalid date. Please use a real date in YYYY-MM-DD format "
                     "(e.g. 2026-08-10).\n";
    }
}

static double readValidAmount(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            clearInputError();
            std::cout << "Invalid amount. Please enter a number (e.g. 500.00).\n";
            continue;
        }
        // Discard the trailing newline left in the buffer after >>.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (value < 0) {
            std::cout << "Amount cannot be negative. Please try again.\n";
            continue;
        }
        return value;
    }
}

// Reads an integer menu choice, re-prompting on invalid (non-numeric) input.
static int readValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            clearInputError();
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

// Reads a category name, re-prompting until it matches one of the
// fixed categories in EXPENSE_CATEGORIES. Returns the normalized name.
static std::string readValidCategory() {
    std::string input, normalized;
    while (true) {
        printCategoryList();
        std::cout << "Enter category: ";
        std::getline(std::cin, input);

        if (isValidCategory(input, normalized)) {
            return normalized;
        }
        std::cout << "Invalid category. Please choose one from the list above.\n\n";
    }
}

// -------------------------------------------------------------
// Menu display
// -------------------------------------------------------------

static void printMainMenu() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "                 SMART EXPENSE TRACKER\n";
    std::cout << std::string(60, '=') << "\n\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View All Expenses\n";
    std::cout << "3. Search Expenses\n";
    std::cout << "4. Update Expense\n";
    std::cout << "5. Delete Expense\n";
    std::cout << "6. Set Monthly Budget\n";
    std::cout << "7. View Budget Status\n";
    std::cout << "8. View Expense Report\n";
    std::cout << "0. Exit\n";
    std::cout << "\n" << std::string(60, '-') << "\n";
}

// -------------------------------------------------------------
// Feature handlers - each function corresponds to one menu option.
// Keeping them separate keeps main() short and readable.
// -------------------------------------------------------------

static void handleAddExpense(ExpenseManager& manager) {
    std::cout << "\n--- Add Expense ---\n";

    std::string date = readValidDate("Enter date (YYYY-MM-DD): ");
    std::string category = readValidCategory();
    std::string description = readNonEmptyLine("Enter description: ");
    double amount = readValidAmount("Enter amount: ");

    int newId = manager.addExpense(date, category, description, amount);
    std::cout << "\nExpense added successfully. (ID: " << newId << ")\n";
}

static void handleViewExpenses(const ExpenseManager& manager) {
    std::cout << "\n--- All Expenses ---\n";
    manager.viewExpenses();
}

static void handleSearchExpenses(const ExpenseManager& manager) {
    std::cout << "\n--- Search Expenses ---\n";
    std::string keyword = readNonEmptyLine("Enter search term (description, category, or date): ");
    manager.searchExpenses(keyword);
}

static void handleUpdateExpense(ExpenseManager& manager) {
    std::cout << "\n--- Update Expense ---\n";
    int id = readValidInt("Enter expense ID to update: ");

    Expense* existing = manager.findExpenseById(id);
    if (existing == nullptr) {
        std::cout << "No expense found with ID " << id << ".\n";
        return;
    }

    std::cout << "\nCurrent details:\n";
    existing->printAsRow();

    std::cout << "\nEnter new details:\n";
    std::string date = readValidDate("Enter date (YYYY-MM-DD): ");
    std::string category = readValidCategory();
    std::string description = readNonEmptyLine("Enter description: ");
    double amount = readValidAmount("Enter amount: ");

    manager.updateExpense(id, date, category, description, amount);
    std::cout << "\nExpense updated successfully.\n";
}

static void handleDeleteExpense(ExpenseManager& manager) {
    std::cout << "\n--- Delete Expense ---\n";
    int id = readValidInt("Enter expense ID to delete: ");

    Expense* existing = manager.findExpenseById(id);
    if (existing == nullptr) {
        std::cout << "No expense found with ID " << id << ".\n";
        return;
    }

    std::cout << "\nExpense to delete:\n";
    existing->printAsRow();

    std::string confirm = readNonEmptyLine("Delete this expense? (y/n): ");
    if (confirm == "y" || confirm == "Y") {
        manager.deleteExpense(id);
        std::cout << "Expense deleted successfully.\n";
    } else {
        std::cout << "Delete cancelled.\n";
    }
}

static void handleSetBudget(ExpenseManager& manager) {
    std::cout << "\n--- Set Monthly Budget ---\n";
    double budget = readValidAmount("Enter monthly budget: ");
    manager.setBudget(budget);
    std::cout << "Monthly budget set successfully.\n";

    if (manager.getTotalSpent() > budget) {
        std::cout << "\nWARNING: Monthly budget exceeded!\n";
    }
}

static void handleViewBudgetStatus(const ExpenseManager& manager) {
    std::cout << "\n";
    manager.viewBudgetStatus();
}

static void handleViewReport(const ExpenseManager& manager) {
    std::cout << "\n";
    Report::generateCategoryReport(manager.getExpenses());
}

// -------------------------------------------------------------
// main()
// -------------------------------------------------------------

int main() {
    // ExpenseManager automatically loads any previously saved data
    // (expenses + budget) as soon as it is constructed.
    ExpenseManager manager;

    bool running = true;
    while (running) {
        printMainMenu();
        int choice = readValidInt("Enter your choice: ");

        switch (choice) {
            case 1: handleAddExpense(manager); break;
            case 2: handleViewExpenses(manager); break;
            case 3: handleSearchExpenses(manager); break;
            case 4: handleUpdateExpense(manager); break;
            case 5: handleDeleteExpense(manager); break;
            case 6: handleSetBudget(manager); break;
            case 7: handleViewBudgetStatus(manager); break;
            case 8: handleViewReport(manager); break;
            case 0:
                std::cout << "\nThank you for using Smart Expense Tracker. Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "\nInvalid choice. Please select an option from the menu.\n";
                break;
        }
    }

    return 0;
}
