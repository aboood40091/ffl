#ifndef FFLI_TEXTURE_H_
#define FFLI_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiTexturePartsType.h>

#include <common/aglTextureData.h>

class FFLiResourceLoader;
class FFLiResourceManager;

FFLResult FFLiLoadTextureWithAllocate(agl::TextureData** ppTextureData, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader);
void FFLiDeleteTexture(agl::TextureData** ppTextureData, bool isExpand);

#endif // FFLI_TEXTURE_H_
