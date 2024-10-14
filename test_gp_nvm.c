#include <stdio.h>
#include <string.h>

#include "gp_nvm.h"

void test_gpNvm() {
    UInt8 length;
    UInt8 value[256];

    printf("Starting tests...\n");

    // Test 1: Set an attribute
    UInt8 value1[] = {1, 2, 3, 4, 5};
    gpNvm_Result result = gpNvm_SetAttribute(1, sizeof(value1), value1);
    if (result == GP_NVM_SUCCESS) {
        printf("Test 1 passed: Set attribute.\n");
    } else {
        printf("Test 1 failed: Unable to set attribute.\n");
    }

    // Test 2: Get the same attribute
    result = gpNvm_GetAttribute(1, &length, value);
    if (result == GP_NVM_SUCCESS && length == sizeof(value1) && memcmp(value, value1, length) == 0) {
        printf("Test 2 passed: Get attribute.\n");
    } else {
        printf("Test 2 failed: Unable to get attribute.\n");
    }

    // Test 3: Update the attribute
    UInt8 value2[] = {10, 20, 30};
    result = gpNvm_SetAttribute(1, sizeof(value2), value2);
    if (result == GP_NVM_SUCCESS) {
        printf("Test 3 passed: Updated attribute.\n");
    } else {
        printf("Test 3 failed: Unable to update attribute.\n");
    }

    // Test 4: Get the updated attribute
    result = gpNvm_GetAttribute(1, &length, value);
    if (result == GP_NVM_SUCCESS && length == sizeof(value2) && memcmp(value, value2, length) == 0) {
        printf("Test 4 passed: Get updated attribute.\n");
    } else {
        printf("Test 4 failed: Unable to get updated attribute.\n");
    }

    // Test 5: Set a new attribute
    UInt8 value3[] = {100, 200};
    result = gpNvm_SetAttribute(2, sizeof(value3), value3);
    if (result == GP_NVM_SUCCESS) {
        printf("Test 5 passed: Set second attribute.\n");
    } else {
        printf("Test 5 failed: Unable to set second attribute.\n");
    }

    // Test 6: Get the new attribute
    result = gpNvm_GetAttribute(2, &length, value);
    if (result == GP_NVM_SUCCESS && length == sizeof(value3) && memcmp(value, value3, length) == 0) {
        printf("Test 6 passed: Get second attribute.\n");
    } else {
        printf("Test 6 failed: Unable to get second attribute.\n");
    }

    printf("Tests finished.\n");
}

int main() {
    test_gpNvm();
    return 0;
}
