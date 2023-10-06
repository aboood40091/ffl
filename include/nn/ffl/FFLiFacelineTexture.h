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

u32 FFLiGetBufferSizeFacelineTexture(u32 resolution, bool enableMipMap, bool compressTexture);
u32 FFLiGetTempBufferSizeFacelineTexture(u32 resolution, bool enableMipMap, bool compressTexture, FFLiResourceManager* pResourceManager, FFLResourceType resourceType);
u32 FFLiGetCompressBufferSizeFacelineTexture(u32 resolution, bool enableMipMap);

void FFLiInitFacelineTexture(FFLiRenderTexture* pRenderTexture, u32 resolution, bool enableMipMap, bool compressTexture, FFLiBufferAllocator* pAllocator);
FFLResult FFLiInitTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, bool enableMipMap, bool compressTexture, bool compressUseUB, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, FFLiRenderTextureBuffer* pRenderTextureBuffer);

template <typename T>
class FFLiCompressorImpl;
struct FFLiCompressorDescBC1;
typedef FFLiCompressorImpl<FFLiCompressorDescBC1> FFLiCompressorImplBC1;

void FFLiRenderFacelineTexture(FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, FFLiFacelineTextureTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface, FFLiCompressorImplBC1* pCompressorImpl);

#endif // FFLI_FACELINE_TEXTURE_H_
