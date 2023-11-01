#ifndef FFLI_FACELINE_TEXTURE_H_
#define FFLI_FACELINE_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

class   FFLiBufferAllocator;
struct  FFLiCharInfo;
class   FFLiCopySurface;
struct  FFLiFacelineTextureTempObject;
class   FFLiRenderTexture;
struct  FFLiRenderTextureBuffer;
class   FFLiResourceLoader;
class   FFLiResourceManager;
class   FFLiShaderCallback;

u32 FFLiGetBufferSizeFacelineTexture(u32 resolution, bool enableMipMap);
u32 FFLiGetTempBufferSizeFacelineTexture(u32 resolution, bool enableMipMap, FFLiResourceManager* pResourceManager, FFLResourceType resourceType);

void FFLiInitFacelineTexture(FFLiRenderTexture* pRenderTexture, u32 resolution, bool enableMipMap, FFLiBufferAllocator* pAllocator);
FFLResult FFLiInitTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, FFLiRenderTextureBuffer* pRenderTextureBuffer);

void FFLiRenderFacelineTexture(FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, FFLiFacelineTextureTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface);

#endif // FFLI_FACELINE_TEXTURE_H_
