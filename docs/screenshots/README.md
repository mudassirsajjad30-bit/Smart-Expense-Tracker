# Screenshots

Reference images used in the main [README](../../README.md). All screenshots
show the actual compiled program running — not mockups.

| File | Shows |
|---|---|
| `main-menu.png` | The main menu on startup |
| `add-expense.png` | The "Add Expense" flow, including the category list and validation prompts |
| `expense-list.png` | "View All Expenses" — the formatted table and running total |
| `budget-status.png` | "View Budget Status" — budget vs. spending, including the over-budget warning |
| `expense-report.png` | "View Expense Report" — category-wise totals with percentages |

## Regenerating these screenshots

If you change the console output (colors, spacing, wording), retake these
screenshots so the docs stay accurate:

1. Build the project: `make`
2. Run it and go through the relevant menu option: `./expense_tracker`
3. Take a screenshot of your terminal window
4. Save it into this folder using the same filename as the option it replaces
