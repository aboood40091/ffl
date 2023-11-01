#ifndef FFLI_PATH_H_
#define FFLI_PATH_H_

#include <nn/ffl/FFLResult.h>

#include <filedevice/rio_FileDevice.h>

u64 FFLiGetMiiStudioTitleID();

rio::RawErrorCode FFLiGetResourcePath(char* pDst, u32 size, const char* filename);
rio::RawErrorCode FFLiGetDatabasePath(char* pDst, u32 size, u64 titleID, const char* filename);

FFLResult FFLiConvertFSStatusToFFLResult(rio::RawErrorCode status);

#endif // FFLI_PATH_H_
