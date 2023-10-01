#ifndef FFLI_TEXTURE_H_
#define FFLI_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiTexturePartsType.h>

#include <cafe/gx2.h>

class FFLiBufferAllocator;
class FFLiResourceLoader;
class FFLiResourceManager;

u32 FFLiGetTextureMaxSizeWithAlign(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType, FFLiTexturePartsType partsType);

FFLResult FFLiLoadTextureWithAllocate(GX2Texture** ppGX2Texture, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator);

void FFLiInvalidateTexture(GX2Texture* pGX2Texture);

#endif // FFLI_TEXTURE_H_
