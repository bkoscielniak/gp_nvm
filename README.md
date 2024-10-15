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

## Supported Data Types
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

# Introduction to the Non-Volatile Memory (NVM) Storage Module

## 1. Purpose of the Module
The Non-Volatile Memory (NVM) Storage Module is designed to provide a simple, file-based abstraction for non-volatile memory typically found in embedded systems (such as EEPROM, Flash). This module allows users to store and retrieve values associated with unique attribute identifiers, simulating how non-volatile storage might function in an embedded environment.

By modeling the NVM storage using a file, the module allows easy testing and prototyping of persistent memory operations on non-volatile storage without the need for specialized hardware.

## 2. Key Features
Attribute-Based Storage: Each value stored in the NVM is associated with a unique attribute ID. The module allows you to store, retrieve, and update values associated with these IDs.

Support for Multiple Data Types: The module supports various data types for storage, such as:

Simple types like UInt8 and UInt32.
Arrays of data (e.g., an array of UInt8).
Complex structures (e.g., a user-defined struct like gpTestData_t).
Persistence in a File: The module uses a file (nvm_storage.bin) to simulate the behavior of non-volatile memory. Each time a value is written to this file, it is preserved across program executions, simulating how non-volatile memory would retain data even after power loss.

Basic Error Handling: The module includes basic error detection, such as handling cases when an attribute cannot be found or the file cannot be opened. It also supports basic checks for file corruption.

## 3. How It Works
The module operates by reading and writing "entries" to a binary file. Each entry corresponds to an attribute, defined as:

```c
typedef struct {
    gpNvm_AttrId id;        // The unique attribute ID
    UInt8 length;           // The length of the stored value
    UInt8 value[255];       // The value itself (up to 255 bytes)
} gpNvm_Entry;
```

Storing Data: When you call gpNvm_SetAttribute(), the module searches the file to see if the attribute already exists. If it does, it overwrites the entry with the new value. If the attribute does not exist, it appends the new entry to the file.

Retrieving Data: When calling gpNvm_GetAttribute(), the module searches for the attribute by its ID in the file. Once found, it reads the value and copies it to the provided buffer.

## 4. Supported Data Types
The module supports various types of data stored as attributes, including:

Basic Types: Simple types like UInt8 and UInt32 can be stored. When storing multi-byte values (like UInt32), the module handles the serialization into a byte array so that the data is written correctly to the file.

Arrays: Arrays of values, such as UInt8[], can also be stored. The length of the array is specified by the user when calling gpNvm_SetAttribute().

Structures: Complex data types such as structs can be stored as long as the total size of the struct does not exceed the maximum value length (255 bytes). This allows you to store structured data like configuration settings or sensor data.

## 5. Data Integrity and Corruption Handling
In real-world embedded systems, non-volatile memory can sometimes become corrupted due to power failures or memory wear. While this implementation doesn't simulate such failures, it does include basic mechanisms for verifying the integrity of stored data. For instance, when retrieving data, the module checks that the correct attribute ID and length are found before returning the value.

## 6. Extensibility
The module has been designed with extensibility in mind:

Easy Data Type Expansion: New data types can be added by simply modifying the read/write functions to handle additional types, such as floating-point numbers or more complex structures.

File-Based Simulation: While this module uses a file to simulate non-volatile memory, it can be easily adapted for use with actual EEPROM or Flash memory hardware by replacing the file operations (fopen, fread, fwrite, etc.) with corresponding hardware-specific functions.

## 7. Usage Scenarios
The NVM module is useful in a variety of embedded applications, such as:

Configuration Storage: Save device settings that need to persist across reboots (e.g., network settings, user preferences).
Sensor Data Storage: Store sensor readings that need to be retained during system power loss.
Firmware Metadata: Keep track of firmware versions or update flags that are preserved across device resets.