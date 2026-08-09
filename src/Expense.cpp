#include "Expense.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>

// ------------------------------------------------------------------
// Small helper: returns a lowercase copy of a string.
// Used only inside this file for case-insensitive category matching.
// ------------------------------------------------------------------
static std::string toLower(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool isValidCategory(const std::string& category, std::string& normalizedCategory) {
    std::string lowerInput = toLower(category);
    for (const auto& valid : EXPENSE_CATEGORIES) {
        if (toLower(valid) == lowerInput) {
            normalizedCategory = valid;
            return true;
        }
    }
    return false;
}

bool isValidDate(const std::string& date) {
    // Strict format: YYYY-MM-DD
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }

    int year = 0;
    int month = 0;
    int day = 0;

    try {
        year = std::stoi(date.substr(0, 4));
        month = std::stoi(date.substr(5, 2));
        day = std::stoi(date.substr(8, 2));
    } catch (const std::exception&) {
        return false;
    }

    if (year < 1900 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    const bool leapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    const int daysInMonth[] = {
        31, leapYear ? 29 : 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    return day <= daysInMonth[month - 1];
}

void printCategoryList() {
    std::cout << "Available categories:\n";
    for (size_t i = 0; i < EXPENSE_CATEGORIES.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << EXPENSE_CATEGORIES[i] << "\n";
    }
}

// --------------------------- Constructors ---------------------------

Expense::Expense()
    : id(0), date(""), category(""), description(""), amount(0.0) {}

Expense::Expense(int id, const std::string& date, const std::string& category,
                  const std::string& description, double amount)
    : id(id), date(date), category(category), description(description), amount(amount) {}

// ----------------------------- Getters -------------------------------

int Expense::getId() const { return id; }
std::string Expense::getDate() const { return date; }
std::string Expense::getCategory() const { return category; }
std::string Expense::getDescription() const { return description; }
double Expense::getAmount() const { return amount; }

// ----------------------------- Setters -------------------------------

void Expense::setDate(const std::string& newDate) { date = newDate; }
void Expense::setCategory(const std::string& newCategory) { category = newCategory; }
void Expense::setDescription(const std::string& newDescription) { description = newDescription; }
void Expense::setAmount(double newAmount) { amount = newAmount; }

// -------------------------- Persistence -------------------------------

std::string Expense::toFileLine() const {
    // Format: id|date|category|description|amount
    std::ostringstream oss;
    oss << id << '|' << date << '|' << category << '|' << description << '|'
        << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

Expense Expense::fromFileLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;

    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }

    if (fields.size() != 5) {
        throw std::runtime_error("Malformed expense record: " + line);
    }

    int id = std::stoi(fields[0]);
    const std::string& date = fields[1];
    const std::string& category = fields[2];
    const std::string& description = fields[3];
    double amount = std::stod(fields[4]);

    std::string normalizedCategory;
    if (!isValidDate(date)) {
        throw std::runtime_error("Invalid date in expense record: " + line);
    }
    if (!isValidCategory(category, normalizedCategory)) {
        throw std::runtime_error("Invalid category in expense record: " + line);
    }
    if (description.empty() || amount < 0.0) {
        throw std::runtime_error("Invalid expense values: " + line);
    }

    return Expense(id, date, normalizedCategory, description, amount);
}

void Expense::printAsRow() const {
    std::cout << std::left
               << std::setw(5)  << id
               << std::setw(13) << date
               << std::setw(14) << category
               << std::setw(25) << description
               << std::right << std::setw(12) << std::fixed << std::setprecision(2) << amount
               << "\n";
}
