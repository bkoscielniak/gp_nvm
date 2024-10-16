#ifndef GP_NVM_H
#define GP_NVM_H

// Error codes
#define GP_NVM_SUCCESS 0
#define GP_NVM_ERROR 1

#define STORAGE_FILE "nvm_storage.bin"

typedef unsigned char UInt8;
typedef unsigned int UInt32;

typedef UInt8 gpNvm_AttrId;
typedef UInt8 gpNvm_Result;

// Struct to store each attribute in the file
typedef struct {
    gpNvm_AttrId id;        // The unique attribute ID
    UInt8 length;           // The length of the stored value
    UInt8 value[255];       // The value itself (up to 255 bytes)
    UInt8 checksum;         // The checksum for verify the integrity
} gpNvm_Entry;

gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, UInt8 *pLength, void *pValue);
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, UInt8 length, void *pValue);

#endif // GP_NVM_H