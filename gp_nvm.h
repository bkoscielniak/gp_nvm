#ifndef GP_NVM_H
#define GP_NVM_H

// Error codes
#define GP_NVM_SUCCESS 0
#define GP_NVM_ERROR 1

typedef unsigned char UInt8;
typedef unsigned int UInt32;

typedef UInt8 gpNvm_AttrId;
typedef UInt8 gpNvm_Result;

gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, UInt8* pLength, void* pValue);
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, UInt8 length, void* pValue);

#endif // GP_NVM_H