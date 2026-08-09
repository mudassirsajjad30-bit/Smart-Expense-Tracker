#include "ExpenseManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>

namespace fs = std::filesystem;

// ------------------------------------------------------------------
// Local helper: lowercase a string (for case-insensitive searching).
// ------------------------------------------------------------------
static std::string toLowerStr(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Prints the shared table header used by View and Search.
static void printTableHeader() {
    std::cout << std::left
               << std::setw(5)  << "ID"
               << std::setw(13) << "Date"
               << std::setw(14) << "Category"
               << std::setw(25) << "Description"
               << std::right << std::setw(12) << "Amount"
               << "\n";
    std::cout << std::string(69, '-') << "\n";
}

// --------------------------- Constructor ---------------------------

ExpenseManager::ExpenseManager(const std::string& expensesFile, const std::string& budgetFile)
    : monthlyBudget(0.0), budgetMonth(currentMonth()), nextId(1),
      expensesFilePath(expensesFile), budgetFilePath(budgetFile) {
    loadData();
}

std::string ExpenseManager::currentMonth() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &nowTime);
#else
    localtime_r(&nowTime, &localTime);
#endif

    std::ostringstream oss;
    oss << (localTime.tm_year + 1900) << '-'
        << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1);
    return oss.str();
}

double ExpenseManager::getCurrentMonthSpent() const {
    const std::string month = currentMonth();
    double total = 0.0;

    for (const auto& e : expenses) {
        if (e.getDate().size() >= 7 && e.getDate().substr(0, 7) == month) {
            total += e.getAmount();
        }
    }

    return total;
}

void ExpenseManager::refreshNextId() {
    int highest = 0;
    for (const auto& e : expenses) {
        highest = std::max(highest, e.getId());
    }
    nextId = highest + 1;
}

// ----------------------------- Add -----------------------------------

int ExpenseManager::addExpense(const std::string& date, const std::string& category,
                                const std::string& description, double amount) {
    Expense newExpense(nextId, date, category, description, amount);
    expenses.push_back(newExpense);
    ++nextId;
    saveData();
    return newExpense.getId();
}

// ----------------------------- View -----------------------------------

void ExpenseManager::viewExpenses() const {
    if (expenses.empty()) {
        std::cout << "No expenses found.\n";
        return;
    }

    printTableHeader();
    for (const auto& e : expenses) {
        e.printAsRow();
    }
    std::cout << std::string(69, '-') << "\n";
    std::cout << "Total Spent: " << std::fixed << std::setprecision(2)
               << getTotalSpent() << "\n";
}

// ---------------------------- Search ------------------------------------

void ExpenseManager::searchExpenses(const std::string& keyword) const {
    std::string lowerKeyword = toLowerStr(keyword);
    bool found = false;

    printTableHeader();
    for (const auto& e : expenses) {
        bool matches =
            toLowerStr(e.getDescription()).find(lowerKeyword) != std::string::npos ||
            toLowerStr(e.getCategory()).find(lowerKeyword) != std::string::npos ||
            toLowerStr(e.getDate()).find(lowerKeyword) != std::string::npos;

        if (matches) {
            e.printAsRow();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No matching expenses found.\n";
    }
}

// ---------------------------- Update ------------------------------------

Expense* ExpenseManager::findExpenseById(int id) {
    for (auto& e : expenses) {
        if (e.getId() == id) {
            return &e;
        }
    }
    return nullptr;
}

bool ExpenseManager::updateExpense(int id, const std::string& date, const std::string& category,
                                    const std::string& description, double amount) {
    Expense* target = findExpenseById(id);
    if (target == nullptr) {
        return false;
    }

    target->setDate(date);
    target->setCategory(category);
    target->setDescription(description);
    target->setAmount(amount);

    saveData();
    return true;
}

// ---------------------------- Delete ------------------------------------

bool ExpenseManager::deleteExpense(int id) {
    auto it = std::find_if(expenses.begin(), expenses.end(),
                            [id](const Expense& e) { return e.getId() == id; });

    if (it == expenses.end()) {
        return false;
    }

    expenses.erase(it);
    saveData();
    return true;
}

// ---------------------------- Budget ------------------------------------

void ExpenseManager::setBudget(double budget) {
    budgetMonth = currentMonth();
    monthlyBudget = budget;
    saveData();
}

double ExpenseManager::getBudget() const {
    return monthlyBudget;
}

double ExpenseManager::getTotalSpent() const {
    double total = 0.0;
    for (const auto& e : expenses) {
        total += e.getAmount();
    }
    return total;
}

void ExpenseManager::viewBudgetStatus() const {
    double spent = getCurrentMonthSpent();
    double remaining = monthlyBudget - spent;

    std::cout << std::string(60, '=') << "\n";
    std::cout << "                     BUDGET STATUS\n";
    std::cout << std::string(60, '=') << "\n\n";

    std::cout << std::left << std::setw(16) << "Monthly Budget" << ": "
               << std::fixed << std::setprecision(2) << monthlyBudget << "\n";
    std::cout << std::left << std::setw(16) << "Total Spent" << ": "
               << std::fixed << std::setprecision(2) << spent << "\n";
    std::cout << std::left << std::setw(16) << "Remaining" << ": "
               << std::fixed << std::setprecision(2) << remaining << "\n";

    std::cout << std::left << std::setw(16) << "Status" << ": ";
    if (monthlyBudget <= 0.0) {
        std::cout << "No budget set\n";
    } else if (spent > monthlyBudget) {
        std::cout << "Over Budget\n";
    } else {
        std::cout << "Within Budget\n";
    }

    std::cout << std::string(60, '=') << "\n";

    if (monthlyBudget > 0.0 && spent > monthlyBudget) {
        std::cout << "\nWARNING: Monthly budget exceeded!\n";
    }
}

// -------------------------- Accessors -----------------------------------

const std::vector<Expense>& ExpenseManager::getExpenses() const {
    return expenses;
}

// -------------------------- Persistence -----------------------------------

void ExpenseManager::loadData() {
    // --- Load expenses -----------------------------------------------
    std::ifstream expenseFile(expensesFilePath);
    if (expenseFile.is_open()) {
        std::string line;
        while (std::getline(expenseFile, line)) {
            if (line.empty()) continue;
            try {
                expenses.push_back(Expense::fromFileLine(line));
            } catch (const std::exception&) {
                // Skip any corrupted line instead of crashing the program.
                std::cerr << "Warning: skipped a corrupted expense record.\n";
            }
        }
        expenseFile.close();
    }
    // If the file does not exist yet, that's fine - it will be created
    // the first time saveData() runs (e.g. after adding an expense).

    refreshNextId();

    // --- Load budget ---------------------------------------------------
    // New format: YYYY-MM|amount
    // Old projects may contain only a number; treat that value as this month's
    // budget so upgrading does not silently lose the user's saved budget.
    std::ifstream budgetFile(budgetFilePath);
    if (budgetFile.is_open()) {
        std::string line;
        if (std::getline(budgetFile, line) && !line.empty()) {
            const std::size_t separator = line.find('|');

            try {
                if (separator != std::string::npos) {
                    const std::string savedMonth = line.substr(0, separator);
                    const double savedBudget = std::stod(line.substr(separator + 1));

                    if (savedMonth == currentMonth() && savedBudget >= 0.0) {
                        budgetMonth = savedMonth;
                        monthlyBudget = savedBudget;
                    } else {
                        // A previous month's budget should not carry into the
                        // new month.
                        budgetMonth = currentMonth();
                        monthlyBudget = 0.0;
                    }
                } else {
                    const double legacyBudget = std::stod(line);
                    if (legacyBudget >= 0.0) {
                        monthlyBudget = legacyBudget;
                        budgetMonth = currentMonth();
                    }
                }
            } catch (const std::exception&) {
                std::cerr << "Warning: invalid budget record; using 0.00 for the current month.\n";
                monthlyBudget = 0.0;
                budgetMonth = currentMonth();
            }
        }
        budgetFile.close();
    }
}

void ExpenseManager::saveData() const {
    // Make sure the "data" directory exists before writing into it.
    fs::path expensePath(expensesFilePath);
    if (expensePath.has_parent_path()) {
        std::error_code ec;
        fs::create_directories(expensePath.parent_path(), ec);
    }

    std::ofstream expenseFile(expensesFilePath, std::ios::trunc);
    if (!expenseFile.is_open()) {
        std::cerr << "Error: could not open " << expensesFilePath << " for writing.\n";
        return;
    }
    for (const auto& e : expenses) {
        expenseFile << e.toFileLine() << "\n";
    }
    expenseFile.close();

    std::ofstream budgetFile(budgetFilePath, std::ios::trunc);
    if (!budgetFile.is_open()) {
        std::cerr << "Error: could not open " << budgetFilePath << " for writing.\n";
        return;
    }
    budgetFile << budgetMonth << "|" << std::fixed << std::setprecision(2) << monthlyBudget << "\n";
    budgetFile.close();
}
