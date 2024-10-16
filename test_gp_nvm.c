#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gp_nvm.h"

#define MAX_LENGTH 20

#define ATTR_ID_UINT8 1
#define ATTR_ID_UINT32 2
#define ATTR_ID_ARRAY 3
#define ATTR_ID_STRUCT 4
#define ATTR_ID_TEST 5

typedef struct {
    UInt8 id;
    UInt32 options;
    UInt8 length;
    UInt8 data[MAX_LENGTH];
} gpTestData_t;

// Helper function to delete the storage file
static void reset_storage(void)
{
    remove(STORAGE_FILE);
}

static void test_set_get_uint8(void)
{
    UInt8 value = 42;
    UInt8 length = sizeof(value);
    gpNvm_Result result = gpNvm_SetAttribute(ATTR_ID_UINT8, length, &value);
    assert(result == GP_NVM_SUCCESS);

    UInt8 readValue;
    UInt8 readLength;
    result = gpNvm_GetAttribute(ATTR_ID_UINT8, &readLength, &readValue);
    assert(result == GP_NVM_SUCCESS);
    assert(readValue == value);
    printf("test_set_get_uint8 passed\n");
}

static void test_set_get_uint32(void)
{
    UInt32 value = 123456789;
    UInt8 length = sizeof(UInt32);  // Set correct length for UInt32 (4 bytes)
    gpNvm_Result result = gpNvm_SetAttribute(ATTR_ID_UINT32, length, &value);
    assert(result == GP_NVM_SUCCESS);

    UInt32 readValue;
    UInt8 readLength;
    result = gpNvm_GetAttribute(ATTR_ID_UINT32, &readLength, &readValue);
    assert(result == GP_NVM_SUCCESS);
    assert(readLength == sizeof(UInt32));  // Ensure the size is correct
    assert(readValue == value);
    printf("test_set_get_uint32 passed\n");
}

static void test_set_get_array(void)
{
    UInt8 array[5] = {1, 2, 3, 4, 5};
    UInt8 length = sizeof(array);
    gpNvm_Result result = gpNvm_SetAttribute(ATTR_ID_ARRAY, length, array);
    assert(result == GP_NVM_SUCCESS);

    UInt8 readArray[5];
    UInt8 readLength;
    result = gpNvm_GetAttribute(ATTR_ID_ARRAY, &readLength, readArray);
    assert(result == GP_NVM_SUCCESS);
    assert(readLength == length);  // Ensure the size is correct
    for (int i = 0; i < length; i++) {
        assert(readArray[i] == array[i]);
    }
    printf("test_set_get_array passed\n");
}

static void test_set_get_struct(void)
{
    gpTestData_t testData = {1, 42, 5, {1, 2, 3, 4, 5}};
    UInt8 length = sizeof(testData);
    gpNvm_Result result = gpNvm_SetAttribute(ATTR_ID_STRUCT, length, &testData);
    assert(result == GP_NVM_SUCCESS);

    gpTestData_t readData;
    UInt8 readLength;
    result = gpNvm_GetAttribute(ATTR_ID_STRUCT, &readLength, &readData);
    assert(result == GP_NVM_SUCCESS);
    assert(readData.id == testData.id);
    assert(readData.options == testData.options);
    assert(readData.length == testData.length);
    assert(readLength == length);  // Ensure the size is correct
    for (int i = 0; i < testData.length; i++) {
        assert(readData.data[i] == testData.data[i]);
    }
    printf("test_set_get_struct passed\n");
}

static void test_data_corruption_detection(void)
{
    reset_storage(); // Ensure clean state for the test

    // Step 1: Set a known value
    UInt32 originalValue = 123456789;
    gpNvm_Result result = gpNvm_SetAttribute(ATTR_ID_TEST, sizeof(UInt32), &originalValue);
    assert(result == GP_NVM_SUCCESS);  // Ensure that setting the value succeeded

    // Step 2: Corrupt the data manually by modifying the file
    FILE *file = fopen(STORAGE_FILE, "r+b");
    assert(file != NULL);  // Ensure the file opened successfully

    // Move to the start of the value in the file (skip `id` and `length`, which are 2 bytes)
    fseek(file, 2, SEEK_SET);

    // Corrupt the first byte of the value
    UInt8 corruptByte = 0xFF; 
    fwrite(&corruptByte, sizeof(UInt8), 1, file);  // Write corrupt byte
    fclose(file); // Close the file

    // Step 3: Attempt to get the attribute and check for error
    UInt32 readValue;
    UInt8 length;
    result = gpNvm_GetAttribute(ATTR_ID_TEST, &length, &readValue);
    assert(result == GP_NVM_ERROR);  // Expect GP_NVM_ERROR due to corruption

    // If the test passes, print a message
    printf("Data corruption detected as expected. Test passed!\n");
}

int main(void)
{
    reset_storage(); // Ensure clean state before first tests
    
    // Run tests
    test_set_get_uint8();
    test_set_get_uint32();
    test_set_get_array();
    test_set_get_struct();
    test_data_corruption_detection();

    printf("All tests passed!\n");
    return 0;
}
