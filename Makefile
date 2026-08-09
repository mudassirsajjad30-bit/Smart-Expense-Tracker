# ------------------------------------------------------------------
# Makefile for Smart Expense Tracker
#
# This exists so you don't have to retype the full g++ command every
# time you change a file. See docs/WORKFLOW.md for the full day-to-day
# development workflow.
#
# Usage:
#   make        -> compiles the project (creates ./expense_tracker)
#   make run    -> compiles (if needed) and runs the program
#   make clean  -> deletes the compiled binary
# ------------------------------------------------------------------

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
SRC := src/main.cpp src/Expense.cpp src/ExpenseManager.cpp src/Report.cpp
TARGET := expense_tracker

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
