#include <nn/ffl/FFLiPath.h>

#if RIO_IS_CAFE
#include <sysapp/title.h>
#include <nn/save/save.h>
#else
#ifndef FFL_MLC_PATH
#error "Must define FFL_MLC_PATH macro"
#endif // FFL_MLC_PATH
#include <cstdio>
#endif // RIO_IS_CAFE

u64 FFLiGetMiiStudioTitleID()
{
#if RIO_IS_CAFE
    return _SYSGetSystemApplicationTitleId(SYSTEM_APP_ID_MII_MAKER);
#else
    return 0;
#endif // RIO_IS_CAFE
}

rio::RawErrorCode FFLiGetResourcePath(char* pDst, u32 size, const char* filename)
{
#if RIO_IS_CAFE
    return rio::RawErrorCode(SAVEGetSharedDataTitlePath(0x0005001B10056000ULL, filename, pDst, size));
#else
    int ret = std::snprintf(pDst, size, FFL_MLC_PATH "/sys/title/0005001b/10056000/content/%s", filename);
    if (ret > 0 && (u32)ret < size)
        return rio::RAW_ERROR_OK;
    return rio::RAW_ERROR_FATAL_ERROR;
#endif // RIO_IS_CAFE
}

rio::RawErrorCode FFLiGetDatabasePath(char* pDst, u32 size, u64 titleID, const char* filename)
{
#if RIO_IS_CAFE
    return rio::RawErrorCode(SAVEGetSharedSaveDataPath(titleID, filename, pDst, size));
#else
    RIO_ASSERT(titleID == 0);
    int ret = std::snprintf(pDst, size, FFL_MLC_PATH "/usr/save/00050010/1004a100/user/common/%s", filename);
    if (ret > 0 && (u32)ret < size)
        return rio::RAW_ERROR_OK;
    return rio::RAW_ERROR_FATAL_ERROR;
#endif
}

FFLResult FFLiConvertFSStatusToFFLResult(rio::RawErrorCode status)
{
    if (status == rio::RAW_ERROR_OK)
        return FFL_RESULT_OK;

    return FFL_RESULT_FS_ERROR;
}
