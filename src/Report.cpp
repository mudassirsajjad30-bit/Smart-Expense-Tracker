#include "Report.h"
#include <iostream>
#include <iomanip>
#include <map>

void Report::generateCategoryReport(const std::vector<Expense>& expenses) {
    std::cout << std::string(60, '=') << "\n";
    std::cout << "                    EXPENSE REPORT\n";
    std::cout << std::string(60, '=') << "\n\n";

    if (expenses.empty()) {
        std::cout << "No expenses found.\n";
        std::cout << std::string(60, '=') << "\n";
        return;
    }

    // std::map keeps categories in alphabetical order automatically,
    // which gives us a clean, predictable report every time.
    std::map<std::string, double> categoryTotals;
    double grandTotal = 0.0;

    for (const auto& e : expenses) {
        categoryTotals[e.getCategory()] += e.getAmount();
        grandTotal += e.getAmount();
    }

    for (const auto& entry : categoryTotals) {
        const std::string& category = entry.first;
        double amount = entry.second;
        double percentage = (grandTotal > 0.0) ? (amount / grandTotal) * 100.0 : 0.0;

        std::cout << std::left << std::setw(20) << category
                   << std::right << std::setw(15) << std::fixed << std::setprecision(2) << amount
                   << std::setw(10) << std::setprecision(1) << percentage << "%\n";
    }

    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left << std::setw(20) << "Total Expenses"
               << std::right << std::setw(15) << std::fixed << std::setprecision(2) << grandTotal << "\n";
    std::cout << std::string(60, '-') << "\n";
}
