#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gp_nvm.h"

#define MAX_LENGTH 20

typedef struct {
    UInt8 id;
    UInt32 options;
    UInt8 length;
    UInt8 data[MAX_LENGTH];
} gpTestData_t;

#define ATTR_ID_UINT8 1
#define ATTR_ID_UINT32 2
#define ATTR_ID_ARRAY 3
#define ATTR_ID_STRUCT 4

void test_set_get_uint8() {
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

void test_set_get_uint32() {
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

void test_set_get_array() {
    UInt8 array[5] = {1, 2, 3, 4, 5};
    UInt8 length = sizeof(array);
    gpNvm_Result result = gpNvm_SetAttribute(ATTR_ID_ARRAY, length, array);
    assert(result == GP_NVM_SUCCESS);

    UInt8 readArray[5];
    UInt8 readLength;
    result = gpNvm_GetAttribute(ATTR_ID_ARRAY, &readLength, readArray);
    assert(result == GP_NVM_SUCCESS);
    for (int i = 0; i < length; i++) {
        assert(readArray[i] == array[i]);
    }
    printf("test_set_get_array passed\n");
}

void test_set_get_struct() {
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
    for (int i = 0; i < testData.length; i++) {
        assert(readData.data[i] == testData.data[i]);
    }
    printf("test_set_get_struct passed\n");
}

int main() {
    // Run tests
    test_set_get_uint8();
    test_set_get_uint32();
    test_set_get_array();
    test_set_get_struct();

    printf("All tests passed!\n");
    return 0;
}
