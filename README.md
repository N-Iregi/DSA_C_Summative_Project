# DSA_C_Summative_Project

A collection of Data Structures & Algorithms exercises and implementations in C prepared as a summative project for the DSA course.

## Table of Contents

- [About](#about)
- [Implemented Modules](#implemented-modules)
- [Prerequisites](#prerequisites)
- [Build & Run](#build--run)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## About

This repository contains C implementations of common data structures and algorithms used in the DSA course summative project. The code is intended to be readable, modular, and easy to compile for study and assessment purposes.

## Implemented Modules

(Replace or extend this list to match what's actually in the repo)

- Arrays and dynamic arrays
- Linked lists (singly / doubly)
- Stacks and queues
- Binary trees and binary search trees
- Sorting algorithms (quick sort, merge sort, bubble sort)
- Searching algorithms (binary search, linear search)

## Prerequisites

- A C compiler (GCC or clang)
- make (optional, if a Makefile is provided)

## Build & Run

Compile a single file:

    gcc -std=c11 -Wall -Wextra -o bin/<program-name> src/<file>.c

Compile all C sources in `src/` into a single executable:

    mkdir -p bin
    gcc -std=c11 -Wall -Wextra src/*.c -o bin/run_all

If a Makefile is present, you can usually build with:

    make

Run an executable from the `bin` directory:

    ./bin/<program-name>

Adjust the filenames and targets above to match the content of this repository.

## Project Structure

A suggested structure (update to reflect the actual repo):

- src/        - C source files
- include/    - Header files
- tests/      - Test programs and test data
- bin/        - Compiled executables (not checked in)
- docs/       - Documentation and notes
- README.md   - This file

## Testing

There are no automated tests included by default. To test a module:

1. Read the header and example usage in the corresponding source file.
2. Compile the source file as shown in "Build & Run".
3. Run the produced executable and verify output.

Consider adding a `tests/` directory or a test harness using a unit test framework (e.g., CUnit) for automated verification.

## Contributing

Contributions, issues and feature requests are welcome. Please fork the repository and create a pull request with a clear description of changes. When adding code, try to:

- Follow consistent naming and style
- Add a header comment describing the purpose of the file and functions
- Provide example usage in comments or a small test program

## License

No license specified. If you want others to use or contribute to this project, consider adding a LICENSE file (for example, the MIT license).

---

If you'd like, I can:

- Detect repository contents and automatically update the "Implemented Modules" and "Project Structure" sections to match the code.
- Add a Makefile or sample test harness.
