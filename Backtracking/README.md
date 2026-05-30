# Backtracking Algorithms in C++

A collection of classic **backtracking problems** implemented in **C++**, designed to demonstrate recursive problem-solving techniques and state-space search.

This project includes solutions for:

- **Sudoku Solver**
- **8 Queens Problem**
- **Anagram / Word Combination Solver**

---
## Implemented Problems

### 1. Sudoku Solver
Solves a standard Sudoku puzzle by filling empty cells while respecting all Sudoku constraints.

---

### 2. 8 Queens Problem
Finds valid arrangements of **8 queens on an 8×8 chessboard** such that no two queens attack each other.

---

### 3. Anagram / Word Combination Solver
Generates valid dictionary-based word combinations from a given set of letters using backtracking.

#### Features
- Avoids duplicate combinations
- Supports repeated letters correctly
- Uses pruning to reduce unnecessary recursion
- Checks prefixes to prevent invalid search branches

#### Use Case
Given a collection of letters and a dictionary, the solver finds all valid words or combinations that can be formed.

---

## Project Structure
```bash
.
├── sudoku.cpp
├── 8_Queens.cpp
├── Anagram.cpp
└── README.md
