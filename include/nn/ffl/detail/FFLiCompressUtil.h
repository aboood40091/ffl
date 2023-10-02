#ifndef FFLI_COMPRESS_UTIL_H_
#define FFLI_COMPRESS_UTIL_H_

#include <nn/ffl/types.h>

#include <cafe/gx2.h>

u32 FFLiCalculateUnitTextureResolution(u32 dimension, u32 mipLevel);

void FFLiSetupColorBufferForBC3(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel);
void FFLiSetupColorBufferForBC1(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel);

#endif // FFLI_COMPRESS_UTIL_H_
