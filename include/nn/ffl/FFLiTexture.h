#ifndef FFLI_TEXTURE_H_
#define FFLI_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiTexturePartsType.h>

#include <common/aglTextureData.h>

class FFLiBufferAllocator;
class FFLiResourceLoader;
class FFLiResourceManager;

u32 FFLiGetTextureMaxSizeWithAlign(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType, FFLiTexturePartsType partsType);

FFLResult FFLiLoadTextureWithAllocate(agl::TextureData** ppTextureData, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator);

#endif // FFLI_TEXTURE_H_
