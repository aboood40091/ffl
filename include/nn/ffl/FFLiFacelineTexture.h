#ifndef FFLI_FACELINE_TEXTURE_H_
#define FFLI_FACELINE_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>

class FFLiResourceManager;

u32 FFLiGetBufferSizeFacelineTexture(u32 resolution, bool enableMipMap, bool enableCompressorParam);
u32 FFLiGetTempBufferSizeFacelineTexture(u32 resolution, bool enableMipMap, bool enableCompressorParam, FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
u32 FFLiGetCompressBufferSizeFacelineTexture(u32 resolution, bool enableMipMap);

#endif // FFLI_FACELINE_TEXTURE_H_
