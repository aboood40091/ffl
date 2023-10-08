#ifndef FFLI_PATH_H_
#define FFLI_PATH_H_

#include <nn/ffl/FFLResult.h>

#include <cafe/fs.h>

u64 FFLiGetMiiStudioTitleID();

FSStatus FFLiGetResourcePath(char* pDst, u32 size, const char* filename);
FSStatus FFLiGetDatabasePath(char* pDst, u32 size, u64 titleID, const char* filename);

FFLResult FFLiConvertFSStatusToFFLResult(FSStatus status);

#endif // FFLI_PATH_H_
