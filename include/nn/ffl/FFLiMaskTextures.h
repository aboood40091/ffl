#ifndef FFLI_MASK_TEXTURES_H_
#define FFLI_MASK_TEXTURES_H_

#include <nn/ffl/FFLResourceType.h>

struct FFLiRenderTexture;

struct FFLiMaskTextures
{
    FFLiRenderTexture*  renderTextures[19];
};
NN_STATIC_ASSERT(sizeof(FFLiMaskTextures) == 0x4C);

class FFLiResourceManager;

u32 FFLiGetBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, bool enableCompressorParam);
u32 FFLiGetTempBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, bool enableCompressorParam, FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
u32 FFLiGetCompressBufferSizeMaskTexture(u32 resolution, bool enableMipMap);

#endif // FFLI_MASK_TEXTURES_H_
