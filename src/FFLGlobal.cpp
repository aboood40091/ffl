#include <nn/ffl/FFLGlobal.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiManager.h>

FFLResult FFLInitResEx(void* pBuffer, const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
    return FFLiInitResEx(pBuffer, pInitDesc, pResDesc);
}

void FFLInitResGPUStep(void)
{
    FFLiInitResGPUStep();
}

u32 FFLGetWorkSize(const FFLInitDesc* pInitDesc)
{
    return FFLiGetWorkSize(pInitDesc);
}

FFLResult FFLFlushQuota(void)
{
    return FFLiFlushQuota(false);
}

FFLResult FFLExit(void)
{
    return FFLiExit();
}

BOOL FFLIsAvailable(void)
{
    return FFLiIsAvailable();
}

FFLColor FFLGetFavoriteColor(s32 index)
{
    return FFLiGetFavoriteColor(index);
}
