#include <nn/ffl/FFLGlobal.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiManager.h>

FFLResult FFLInitResEx(const FFLInitDesc* pInitDesc, const FFLResourceDesc* pResDesc)
{
    return FFLiInitResEx(pInitDesc, pResDesc);
}

void FFLInitResGPUStep(void)
{
    FFLiInitResGPUStep();
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
