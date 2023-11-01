#ifndef FFLI_MASK_TEXTURES_H_
#define FFLI_MASK_TEXTURES_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

struct FFLiRenderTexture;

struct FFLiMaskTextures
{
    FFLiRenderTexture*  pRenderTextures[19];
};
NN_STATIC_ASSERT(sizeof(FFLiMaskTextures) == 0x4C);

class   FFLiBufferAllocator;
struct  FFLiCharInfo;
class   FFLiCopySurface;
struct  FFLiMaskTexturesTempObject;
struct  FFLiRenderTextureBuffer;
class   FFLiResourceLoader;
class   FFLiResourceManager;
class   FFLiShaderCallback;

u32 FFLiGetBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap);
u32 FFLiGetTempBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, FFLiResourceManager* pResourceManager, FFLResourceType resourceType);

FFLExpression FFLiInitMaskTextures(FFLiMaskTextures* pMaskTextures, u32 expressionFlag, u32 resolution, bool enableMipMap, FFLiBufferAllocator* pAllocator);
FFLResult FFLiInitTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, const FFLiMaskTextures* pMaskTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, FFLiRenderTextureBuffer* pRenderTextureBuffer);

void FFLiRenderMaskTextures(FFLiMaskTextures* pMaskTextures, FFLiMaskTexturesTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface);

#endif // FFLI_MASK_TEXTURES_H_
