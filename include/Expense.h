#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include <array>

// ------------------------------------------------------------------
// The fixed list of categories accepted by the application.
// Keeping this in one place means the whole program stays in sync
// whenever a category is added, removed, or renamed.
// ------------------------------------------------------------------
const std::array<std::string, 8> EXPENSE_CATEGORIES = {
    "Food", "Transport", "Education", "Shopping",
    "Bills", "Entertainment", "Health", "Other"
};

// Returns true if the given text matches one of EXPENSE_CATEGORIES
// (case-insensitive) and stores the properly-capitalised version in
// 'normalizedCategory'.
bool isValidCategory(const std::string& category, std::string& normalizedCategory);

// Returns true only for a real calendar date in strict YYYY-MM-DD format.
// Leap years and month lengths are validated as well.
bool isValidDate(const std::string& date);

// Prints the list of valid categories, numbered, to the console.
void printCategoryList();

// ------------------------------------------------------------------
// Expense
//
// A single financial transaction entered by the user. This class is
// intentionally simple: it only knows how to store its own data and
// convert itself to/from the flat-file format used for persistence.
// All "business logic" (searching, totals, reports, etc.) lives in
// ExpenseManager / Report so that Expense stays focused on one job.
// ------------------------------------------------------------------
class Expense {
private:
    int id;
    std::string date;          // Expected format: YYYY-MM-DD
    std::string category;      // One of EXPENSE_CATEGORIES
    std::string description;
    double amount;

public:
    // Default constructor (needed so Expense objects can live in a vector
    // and be created before their real values are known, e.g. while parsing).
    Expense();

    Expense(int id, const std::string& date, const std::string& category,
            const std::string& description, double amount);

    // --- Getters -----------------------------------------------------
    int getId() const;
    std::string getDate() const;
    std::string getCategory() const;
    std::string getDescription() const;
    double getAmount() const;

    // --- Setters (used by the "Update Expense" feature) --------------
    void setDate(const std::string& newDate);
    void setCategory(const std::string& newCategory);
    void setDescription(const std::string& newDescription);
    void setAmount(double newAmount);

    // --- Persistence helpers ------------------------------------------
    // Converts this expense into a single line of text for saving to disk.
    // Fields are separated by '|' because none of our fields should ever
    // contain that character.
    std::string toFileLine() const;

    // Parses a line previously produced by toFileLine() back into an
    // Expense object. Throws std::runtime_error if the line is malformed.
    static Expense fromFileLine(const std::string& line);

    // Prints one formatted table row for this expense (used by View/Search).
    void printAsRow() const;
};

#endif // EXPENSE_H
