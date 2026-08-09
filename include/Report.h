#ifndef REPORT_H
#define REPORT_H

#include <vector>
#include "Expense.h"

// ------------------------------------------------------------------
// Report
//
// A small, self-contained helper responsible only for generating the
// category-wise spending report (Feature 8). Keeping it separate from
// ExpenseManager follows the "single responsibility" idea: ExpenseManager
// manages data, Report presents it.
// ------------------------------------------------------------------
class Report {
public:
    // Prints a category-wise breakdown of all expenses, including each
    // category's percentage of total spending.
    static void generateCategoryReport(const std::vector<Expense>& expenses);
};

#endif // REPORT_H
