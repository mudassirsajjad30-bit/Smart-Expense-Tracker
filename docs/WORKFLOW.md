# Development Workflow

A simple, repeatable process for making changes to Smart Expense Tracker —
whether you're fixing a bug, adding a feature, or just experimenting.

## The everyday loop

```
 ┌─────────────┐   ┌───────────┐   ┌────────────┐   ┌──────────────┐
 │ 1. Edit code │ → │ 2. Build  │ → │ 3. Test it │ → │ 4. Commit &  │
 │  (src/ or    │   │  (make)   │   │  by hand   │   │    push      │
 │   include/)  │   │           │   │            │   │              │
 └─────────────┘   └───────────┘   └────────────┘   └──────────────┘
        ▲                                                   │
        └───────────────────────────────────────────────────┘
                        repeat for the next change
```

### 1. Edit code
Change whatever you need in `src/` (implementation) or `include/` (headers).
Small, focused edits are easier to test and easier to explain in a commit
message than one huge change.

### 2. Build
```bash
make
```
This recompiles only what's needed and produces `expense_tracker`
(or `expense_tracker.exe` on Windows). If you'd rather build and run in
one step:
```bash
make run
```
No `Makefile`? The direct command still works:
```bash
g++ -std=c++17 src/main.cpp src/Expense.cpp src/ExpenseManager.cpp src/Report.cpp -Iinclude -o expense_tracker
```

### 3. Test it by hand
Run the program and walk through whatever you changed, plus a couple of
related menu options, to make sure nothing else broke:
```bash
./expense_tracker
```
A quick personal checklist:
- Does the feature you changed behave correctly?
- Does it reject bad input instead of crashing (empty text, negative
  amounts, letters where numbers are expected)?
- Close the program and reopen it — is your data still there?

### 4. Commit & push
```bash
git add .
git commit -m "Short, clear description of what changed"
git push
```

**Good commit message examples:**
- `Add validation for expense dates`
- `Fix budget percentage rounding in report`
- `Add CSV export option to main menu`

**Avoid:**
- `update` / `fix stuff` / `final final v2`

## Working with branches (optional, but good practice)

For anything bigger than a one-line fix, isolate it on its own branch so
`main` always stays in a working state:

```bash
git checkout -b feature/csv-export   # create and switch to a new branch
# ... edit, build, test ...
git add .
git commit -m "Add CSV export option"
git push -u origin feature/csv-export
```

Then open a Pull Request on GitHub to merge it into `main` once you're happy
with it — even on a solo project, this gives you a clean history and a
place to review your own diff before it becomes permanent.

## Before every commit, ask yourself

| Question | Why it matters |
|---|---|
| Does `make` finish with no warnings? | Warnings often point to real bugs |
| Did I test the feature I changed? | Untested code is the #1 source of regressions |
| Did I test at least one *other* menu option? | Catches accidental side effects |
| Is my commit message clear? | Future-you will thank present-you |
| Did I avoid committing `expense_tracker` / `expense_tracker.exe`? | `.gitignore` already handles this — don't force-add binaries |
