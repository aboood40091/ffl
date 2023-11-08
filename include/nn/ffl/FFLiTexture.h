#ifndef FFLI_TEXTURE_H_
#define FFLI_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiTexturePartsType.h>

#include <gpu/rio_Texture.h>

class FFLiResourceLoader;
class FFLiResourceManager;

FFLResult FFLiLoadTextureWithAllocate(rio::Texture2D** ppTexture2D, FFLiTexturePartsType partsType, u32 index, FFLiResourceLoader* pResLoader);
void FFLiDeleteTexture(rio::Texture2D** ppTexture2D, bool isExpand);

#endif // FFLI_TEXTURE_H_
