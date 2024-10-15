#include <stdio.h>
#include <string.h>
#include "gp_nvm.h"

#define STORAGE_FILE "nvm_storage.bin"
#define MAX_ATTRIBUTES 100

// Struct to store each attribute in the file
typedef struct {
    gpNvm_AttrId id;
    UInt8 length;
    UInt8 value[255]; // Max size for the value
} gpNvm_Entry;

// Helper function to check if the file exists
static int file_exists() {
    FILE *file = fopen(STORAGE_FILE, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Function to handle writing multi-byte values like UInt32 to file (Endian-agnostic)
void write_uint32_to_buffer(UInt8* buffer, UInt32 value) {
    buffer[0] = (value >> 24) & 0xFF;
    buffer[1] = (value >> 16) & 0xFF;
    buffer[2] = (value >> 8) & 0xFF;
    buffer[3] = value & 0xFF;
}

// Function to handle reading multi-byte values like UInt32 from file (Endian-agnostic)
UInt32 read_uint32_from_buffer(const UInt8* buffer) {
    return ((UInt32)buffer[0] << 24) |
           ((UInt32)buffer[1] << 16) |
           ((UInt32)buffer[2] << 8) |
           (UInt32)buffer[3];
}

// Function to get an attribute value from non-volatile memory
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, UInt8* pLength, void* pValue) {
    if (!file_exists()) {
        printf("Error: File not found.\n");
        return GP_NVM_ERROR;
    }

    FILE *file = fopen(STORAGE_FILE, "rb");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return GP_NVM_ERROR;
    }

    gpNvm_Entry entry;
    while (fread(&entry, sizeof(gpNvm_Entry), 1, file)) {
        if (entry.id == attrId) {
            *pLength = entry.length;
            if (entry.length == sizeof(UInt32)) {
                UInt32 value = read_uint32_from_buffer(entry.value);
                memcpy(pValue, &value, sizeof(UInt32));
            } else {
                memcpy(pValue, entry.value, entry.length);
            }
            fclose(file);
            //printf("Success: Attribute %d read with length %d\n", attrId, *pLength);
            return GP_NVM_SUCCESS;
        }
    }

    fclose(file);
    printf("Error: Attribute %d not found.\n", attrId);
    return GP_NVM_ERROR; // Attribute not found
}

// Function to set an attribute value in non-volatile memory
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, UInt8 length, void* pValue) {
    FILE *file;
    gpNvm_Entry entry;
    int found = 0;

    // If file exists, open in read+write mode. Else, create it.
    if (file_exists()) {
        file = fopen(STORAGE_FILE, "r+b");
    } else {
        file = fopen(STORAGE_FILE, "wb+");
    }

    if (!file) {
        printf("Error: Unable to open or create file.\n");
        return GP_NVM_ERROR;
    }

    // Search for the existing attribute ID in the file
    while (fread(&entry, sizeof(gpNvm_Entry), 1, file)) {
        if (entry.id == attrId) {
            found = 1;
            fseek(file, -sizeof(gpNvm_Entry), SEEK_CUR); // Move back to overwrite
            break;
        }
    }

    // Prepare the entry to write
    entry.id = attrId;

    // If setting a UInt32 value, ensure the length is 4 bytes
    if (length == sizeof(UInt32)) {
        entry.length = sizeof(UInt32);  // Correct length for UInt32
        write_uint32_to_buffer(entry.value, *((UInt32*)pValue));
    } else {
        entry.length = length;
        memcpy(entry.value, pValue, length);
    }

    // Write the entry (overwrite if found, append if new)
    size_t result = fwrite(&entry, sizeof(gpNvm_Entry), 1, file);
    if (result != 1) {
        printf("Error: Writing to file failed.\n");
        fclose(file);
        return GP_NVM_ERROR;
    }

    fclose(file);
    //printf("Success: Attribute %d written with length %d\n", attrId, entry.length);
    return GP_NVM_SUCCESS;
}
