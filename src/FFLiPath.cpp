#include <nn/ffl/FFLiPath.h>

#include <cafe/sysapp.h>

#include <nn/save/save_Types.h>

extern "C" {

// Thanks for deprecating these, Nintendo... :/

typedef enum SysSystemApplicationID
{
    SYS_ID_MII_MAKER    = 4
} SysSystemApplicationID;

u64 _SYSGetSystemApplicationTitleId(SysSystemApplicationID id);

SAVEStatus SAVEGetSharedDataTitlePath(u64 titleId, const char* path, char* dst, u32 size);
SAVEStatus SAVEGetSharedSaveDataPath(u64 titleId, const char* path, char* dst, u32 size);

}

u64 FFLiGetMiiStudioTitleID()
{
    return _SYSGetSystemApplicationTitleId(SYS_ID_MII_MAKER);
}

FSStatus FFLiGetResourcePath(char* pDst, u32 size, const char* filename)
{
    return SAVEGetSharedDataTitlePath(0x0005001B10056000ULL, filename, pDst, size);
}

FSStatus FFLiGetDatabasePath(char* pDst, u32 size, u64 titleID, const char* filename)
{
    return SAVEGetSharedSaveDataPath(titleID, filename, pDst, size);
}

FFLResult FFLiConvertFSStatusToFFLResult(FSStatus status)
{
    if (status == FS_STATUS_OK)
        return FFL_RESULT_OK;

    return FFL_RESULT_FS_ERROR;
}
