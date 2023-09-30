#ifndef FFLI_TEXTURE_H_
#define FFLI_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>

#include <nn/ffl/FFLiTexturePartsType.h>

class FFLiResourceManager;

u32 FFLiGetTextureMaxSizeWithAlign(const FFLiResourceManager* pResourceManager, FFLResourceType resourceType, FFLiTexturePartsType partsType);

#endif // FFLI_TEXTURE_H_
