#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include <vector>
#include <string>
#include "Expense.h"

// ------------------------------------------------------------------
// ExpenseManager
//
// This is the "brain" of the application. It owns the full list of
// expenses and the monthly budget, and it is responsible for every
// operation that changes or reads that data: adding, searching,
// updating, deleting, budgeting, and saving/loading from disk.
//
// main.cpp only talks to ExpenseManager - it never touches the
// expense file or the vector directly. This keeps the menu code
// simple and keeps all the "real" logic in one testable place.
// ------------------------------------------------------------------
class ExpenseManager {
private:
    std::vector<Expense> expenses;
    double monthlyBudget;
    std::string budgetMonth;       // Month the saved budget belongs to (YYYY-MM)
    int nextId;                    // Next auto-generated expense ID

    std::string expensesFilePath;
    std::string budgetFilePath;

    // Recalculates nextId based on the highest ID currently loaded.
    void refreshNextId();
    static std::string currentMonth();
    double getCurrentMonthSpent() const;

public:
    // Paths default to the standard project locations but can be
    // overridden (useful for testing).
    explicit ExpenseManager(const std::string& expensesFile = "data/expenses.txt",
                             const std::string& budgetFile = "data/budget.txt");

    // --- Feature 1: Add ------------------------------------------------
    // Returns the newly created expense's ID.
    int addExpense(const std::string& date, const std::string& category,
                   const std::string& description, double amount);

    // --- Feature 2: View -------------------------------------------------
    void viewExpenses() const;

    // --- Feature 3: Search -----------------------------------------------
    // Searches description, category and date (partial, case-insensitive match).
    void searchExpenses(const std::string& keyword) const;

    // --- Feature 4: Update -----------------------------------------------
    bool updateExpense(int id, const std::string& date, const std::string& category,
                        const std::string& description, double amount);

    // --- Feature 5: Delete -----------------------------------------------
    bool deleteExpense(int id);

    // Finds an expense by ID. Returns nullptr if not found.
    Expense* findExpenseById(int id);

    // --- Feature 6 & 7: Budget -------------------------------------------
    void setBudget(double budget);
    double getBudget() const;
    double getTotalSpent() const;
    void viewBudgetStatus() const;

    // Read-only access to the full list (used by Report).
    const std::vector<Expense>& getExpenses() const;

    // --- Feature 9: Persistence --------------------------------------------
    void loadData();
    void saveData() const;
};

#endif // EXPENSE_MANAGER_H
