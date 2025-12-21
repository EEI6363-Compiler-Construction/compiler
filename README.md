# Simple Compiler (Course Project)

**A small compiler implemented as a course design project for EEX6363 — Compiler Construction.**

---

## Overview
This repository contains a hand-written compiler front-end (lexical, syntax, and semantic analysis) and basic code generation components. It is implemented in C, uses Flex for lexical analysis, and demonstrates parsing, AST construction, symbol table management, semantic checks, and simple code generation.

---

## Features
- Lexical analysis using **Flex** (generator included in `build.sh`)
- Top-down parsing with a parsing table and custom parsing stack
- AST construction and printing (`output/ast.txt`)
- Symbol table implementation and pretty-printer (`output/symbol_table.txt`)
- Basic semantic analysis and type checking
- Simple code generation (see `output/tserget_code.txt`)

---

## Repository structure
- `src/` — source code
  - `lexical_analyzer/` — `.l` file and generated scanner
  - `syntax_analyzer/` — parser, parsing stack, parse tables, AST
  - `semantic_analyzer/` — semantic checks, symbol table
  - `utils/` — helpers (logger, token strings, error handling)
  - `main.c` — compiler entry point
- `build.sh` — builds the project (runs `flex` + `gcc`) 
- `run.sh` — runs the compiled binary on `tests/prg.txt`
- `tests/` — sample test programs
- `output/` — generated artifacts (`ast.txt`, `symbol_table.txt`, `target_code.txt`)


---

## Prerequisites
- Linux
- GCC
- Flex (lex)

Install on Ubuntu/Debian: `sudo apt install build-essential flex`.

---

## Build & Run
1. Build the project (will generate the scanner and compile C files):

```sh
./build.sh
```

2. Run the compiled compiler on the default test program:

```sh
./run.sh
```

Or run directly with an input file:

```sh
./build/compiler path/to/your_program.txt
```


---

## Tests & Example
- Example program: `tests/prg.txt`
- After running, inspect `output/ast.txt`, `output/symbol_table.txt`, and `output/target_code.txt` for produced artifacts.

---


## Course Info
Course project for EEX6363 — Compiler Construction.

---

