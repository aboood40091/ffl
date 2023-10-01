#ifndef FFLI_MASK_TEXTURES_H_
#define FFLI_MASK_TEXTURES_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

struct FFLiRenderTexture;

struct FFLiMaskTextures
{
    FFLiRenderTexture*  renderTextures[19];
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

u32 FFLiGetBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, bool enableCompressorParam);
u32 FFLiGetTempBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, bool enableCompressorParam, FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
u32 FFLiGetCompressBufferSizeMaskTexture(u32 resolution, bool enableMipMap);

FFLExpression FFLiInitMaskTextures(FFLiMaskTextures* pMaskTextures, u32 expressionFlag, u32 resolution, bool enableMipMap, bool enableCompressorParam, FFLiBufferAllocator* pAllocator);
FFLResult FFLiInitTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, const FFLiMaskTextures* pMaskTextures, const FFLiCharInfo* pInfo, u32 expressionFlag, u32 resolution, bool enableMipMap, bool enableCompressorParam, bool useCompressorUB, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, FFLiRenderTextureBuffer* pRenderTextureBuffer);

template <typename T>
class FFLiCompressorImpl;
struct FFLiCompressorDescBC3;
typedef FFLiCompressorImpl<FFLiCompressorDescBC3> FFLiCompressorImplBC3;

void FFLiRenderMaskTextures(FFLiMaskTextures* pMaskTextures, FFLiMaskTexturesTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface, FFLiCompressorImplBC3* pCompressorImpl);

#endif // FFLI_MASK_TEXTURES_H_
