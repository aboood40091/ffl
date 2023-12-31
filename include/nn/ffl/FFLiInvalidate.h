#ifndef FFLI_INVALIDATE_H_
#define FFLI_INVALIDATE_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

void FFLiInvalidate(GX2InvalidateType invalidateType, void* ptr, u32 size);
void FFLiInvalidateSurface(GX2Surface* pSurface, GX2InvalidateType invalidateType);
void FFLiInvalidateColorBuffer(GX2ColorBuffer* pColorBuffer);
void FFLiInvalidateShader(void* ptr, u32 size);

#endif // FFLI_INVALIDATE_H_
