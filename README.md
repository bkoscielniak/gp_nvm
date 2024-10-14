# Non-Volatile Memory Storage Component

This project implements a non-volatile memory storage component in C. It simulates non-volatile memory using a file and provides functions to set and get attributes by their IDs. It also includes basic error detection for file corruption.

## Features

- Supports setting and getting attributes by ID.
- Simulates non-volatile memory using a file (`nvm_storage.bin`).
- Detects basic corruption in the file.
- Includes unit tests to verify functionality.

## Files

- `gp_nvm.c`: Implementation of the non-volatile memory component.
- `gp_nvm.h`: Header file containing function declarations and definitions.
- `test_gp_nvm.c`: Unit tests for the `gp_nvm` implementation.
- `Makefile`: Simple build script to compile the project.
- `README.md`: This file, containing usage instructions.

## Build and Run

To compile and run the unit tests, follow these steps:

### Prerequisites

- **Compiler:** Ensure you have `gcc` installed or any other C compiler.
- **Make:** Make sure you have `make` installed to use the provided `Makefile` (or compile manually with `gcc`).

### Step 1: Clone the Repository

```bash
git clone https://github.com/bkoscielniak/gp_nvm.git
cd gp_nvm
```

### Step 2: Compile the Code
A Makefile is provided to make compilation easier. To compile the project, simply run:

```bash
make
```

This will compile both the main implementation (gp_nvm.c) and the unit tests (test_gp_nvm.c), and create an executable test_gp_nvm.

### Step 3: Run Unit Tests
Once compiled, you can run the unit tests using the following command:

```bash
./test_gp_nvm
```

The output will show whether each test case passes or fails.

### Step 4: Clean the Build
To clean the project and remove the compiled files, you can run:

```bash
make clean
```

This will delete the test_gp_nvm executable and any object files.

### Example Output

```bash
Starting tests...
Test 1 passed: Set attribute.
Test 2 passed: Get attribute.
Test 3 passed: Updated attribute.
Test 4 passed: Get updated attribute.
Test 5 passed: Set second attribute.
Test 6 passed: Get second attribute.
Tests finished.
```

### Manual Compilation (Optional)
If you prefer not to use make, you can compile the project manually using gcc:

```bash
gcc -o test_gp_nvm gp_nvm.c test_gp_nvm.c
./test_gp_nvm
```

This will produce the same result as using the Makefile.
