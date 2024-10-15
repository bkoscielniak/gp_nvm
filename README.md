# Non-Volatile Memory (NVM) Storage Component

## Overview

This project provides a simple implementation of a non-volatile memory (NVM) storage component in C. The underlying storage is modeled as a file, and the component can back up and restore values corresponding to unique attribute identifiers.

The implementation supports:
- Basic data types like `UInt8`, `UInt32`.
- Arrays of `UInt8`.
- Structs with arbitrary data (e.g., `gpTestData_t` struct).
- Detection of storage corruption (through basic file operations).

## File Structure

- `gp_nvm.h`: Header file defining the NVM API.
- `gp_nvm.c`: Implementation of the NVM functions.
- `test_gp_nvm.c`: Unit tests to verify the functionality of the NVM component.
- `nvm_storage.bin`: File used as the non-volatile memory (created at runtime).

## How to Compile

To compile the project and run the unit tests, follow these steps:

### 1. Compile the Code

```bash
gcc -o test_gp_nvm test_gp_nvm.c gp_nvm.c
```

This will create an executable called test_gp_nvm.

### 2. Run Unit Tests
After compilation, you can run the unit tests using the following command:

```bash
./test_gp_nvm
```

### 3. Expected Output
You should see the following output if all tests pass:

```css
test_set_get_uint8 passed
test_set_get_uint32 passed
test_set_get_array passed
test_set_get_struct passed
All tests passed!
```

### Supported Data Types
The component supports the following data types for pValue:

- Basic types like UInt8, UInt32.
- Arrays of UInt8.
- Structs like gpTestData_t:
```c
#define MAX_LENGTH 20
typedef struct {
    UInt8  id;
    UInt32 options;
    UInt8  length;
    UInt8  data[MAX_LENGTH];
} gpTestData_t;
```