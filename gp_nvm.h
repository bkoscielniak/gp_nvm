#ifndef GP_NVM_H
#define GP_NVM_H

#define GP_NVM_SUCCESS 0
#define GP_NVM_FAILURE 1

typedef unsigned char UInt8;

typedef UInt8 gpNvm_AttrId;
typedef UInt8 gpNvm_Result;

// Function to get an attribute value from non-volatile memory
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, UInt8* pLength, UInt8* pValue);

// Function to set an attribute value in non-volatile memory
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, UInt8 length, UInt8* pValue);

#endif // GP_NVM_H
