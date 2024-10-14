/**
 * 
 * Data Structure:
 * NvmAttribute: Holds the attribute ID, length, and value.
 * The values are stored in an array of NvmAttribute structures in the file nvm_storage.bin.
 * 
 * gpNvm_GetAttribute:
 * Reads all attributes from the file. Searches for the requested attribute by ID.
 * If found, it copies the value to the caller's buffer and returns success.
 * 
 * gpNvm_SetAttribute:
 * Reads all attributes. Updates the existing attribute if it already exists.
 * Adds a new attribute if it does not exist.
 * 
 * Helper Functions:
 * readAllAttributes: Reads the attributes from the file into memory.
 * writeAllAttributes: Writes all attributes from memory to the file.
 * 
 * Corruption Detection:
 * The code already has a basic check for file corruption by verifying if the file size is a multiple of the NvmAttribute structure size:
 * 
 * if (fileSize % sizeof(NvmAttribute) != 0) {
 *      fclose(file);
 *      return GP_NVM_FAILURE;  // Corrupted storage
 * }
 * 
 * This can be further enhanced by adding CRC checksums or other data integrity mechanisms.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "gp_nvm.h"

#define NVM_FILE "nvm_storage.bin"

// Max size of value to simplify implementation
#define MAX_VALUE_SIZE 256

typedef struct {
    gpNvm_AttrId attrId;
    UInt8 length;
    UInt8 value[MAX_VALUE_SIZE];
} NvmAttribute;

// Helper function to read all stored attributes
static gpNvm_Result readAllAttributes(NvmAttribute** pAttributes, size_t* count) {
    FILE* file = fopen(NVM_FILE, "rb");
    if (!file) {
        *count = 0;
        *pAttributes = NULL;
        return GP_NVM_SUCCESS;  // No file means no data stored yet
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize % sizeof(NvmAttribute) != 0) {
        fclose(file);
        return GP_NVM_FAILURE;  // Corrupted storage
    }

    *count = fileSize / sizeof(NvmAttribute);
    *pAttributes = (NvmAttribute*)malloc(fileSize);

    if (fread(*pAttributes, sizeof(NvmAttribute), *count, file) != *count) {
        fclose(file);
        free(*pAttributes);
        return GP_NVM_FAILURE;
    }

    fclose(file);
    return GP_NVM_SUCCESS;
}

// Helper function to write all attributes
static gpNvm_Result writeAllAttributes(NvmAttribute* attributes, size_t count) {
    FILE* file = fopen(NVM_FILE, "wb");
    if (!file) {
        return GP_NVM_FAILURE;
    }

    if (fwrite(attributes, sizeof(NvmAttribute), count, file) != count) {
        fclose(file);
        return GP_NVM_FAILURE;
    }

    fclose(file);
    return GP_NVM_SUCCESS;
}

// Fetches the attribute by ID
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, UInt8* pLength, UInt8* pValue) {
    NvmAttribute* attributes = NULL;
    size_t count = 0;

    if (readAllAttributes(&attributes, &count) != GP_NVM_SUCCESS) {
        return GP_NVM_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        if (attributes[i].attrId == attrId) {
            *pLength = attributes[i].length;
            memcpy(pValue, attributes[i].value, attributes[i].length);
            free(attributes);
            return GP_NVM_SUCCESS;
        }
    }

    free(attributes);
    return GP_NVM_FAILURE;  // Attribute not found
}

// Sets or updates an attribute by ID
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, UInt8 length, UInt8* pValue) {
    if (length > MAX_VALUE_SIZE) {
        return GP_NVM_FAILURE;  // Value size too large
    }

    NvmAttribute* attributes = NULL;
    size_t count = 0;

    if (readAllAttributes(&attributes, &count) != GP_NVM_SUCCESS) {
        return GP_NVM_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        if (attributes[i].attrId == attrId) {
            // Update existing attribute
            attributes[i].length = length;
            memcpy(attributes[i].value, pValue, length);
            gpNvm_Result result = writeAllAttributes(attributes, count);
            free(attributes);
            return result;
        }
    }

    // Add new attribute
    NvmAttribute* newAttributes = (NvmAttribute*)realloc(attributes, (count + 1) * sizeof(NvmAttribute));
    if (!newAttributes) {
        free(attributes);
        return GP_NVM_FAILURE;
    }

    newAttributes[count].attrId = attrId;
    newAttributes[count].length = length;
    memcpy(newAttributes[count].value, pValue, length);

    gpNvm_Result result = writeAllAttributes(newAttributes, count + 1);
    free(newAttributes);
    return result;
}
