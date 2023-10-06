#include <nn/ffl/FFLiInvalidate.h>

void FFLiInvalidate(GX2InvalidateType invalidateType, void* ptr, u32 size)
{
    GX2Invalidate(invalidateType, ptr, size);
}

void FFLiInvalidateSurface(GX2Surface* pSurface, GX2InvalidateType invalidateType)
{
    if (pSurface->imageSize != 0)
        FFLiInvalidate(invalidateType, pSurface->imagePtr, pSurface->imageSize);

    if (pSurface->mipSize != 0)
        FFLiInvalidate(invalidateType, pSurface->mipPtr, pSurface->mipSize);
}

void FFLiInvalidateColorBuffer(GX2ColorBuffer* pColorBuffer)
{
    FFLiInvalidateSurface(&pColorBuffer->surface, GX2_INVALIDATE_COLOR_BUFFER);
}

void FFLiInvalidateShader(void* ptr, u32 size)
{
    GX2Invalidate(GX2_INVALIDATE_CPU_SHADER, ptr, size);
}
