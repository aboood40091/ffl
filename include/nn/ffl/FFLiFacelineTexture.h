#ifndef FFLI_FACELINE_TEXTURE_H_
#define FFLI_FACELINE_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <misc/rio_Types.h>

struct  FFLiCharInfo;
#if RIO_IS_CAFE
class   FFLiCopySurface;
#endif // RIO_IS_CAFE
struct  FFLiFacelineTextureTempObject;
class   FFLiRenderTexture;
struct  FFLiRenderTextureBuffer;
class   FFLiResourceLoader;
class   FFLiResourceManager;
class   FFLiShaderCallback;

void FFLiInitFacelineTexture(FFLiRenderTexture* pRenderTexture, u32 resolution, bool enableMipMap);
void FFLiDeleteFacelineTexture(FFLiRenderTexture* pRenderTexture);

FFLResult FFLiInitTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiRenderTextureBuffer* pRenderTextureBuffer);
void FFLiDeleteTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, const FFLiCharInfo* pCharInfo, FFLResourceType resourceType);

void FFLiRenderFacelineTexture(FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, FFLiFacelineTextureTempObject* pObject, const FFLiShaderCallback* pCallback
#if RIO_IS_CAFE
    , FFLiCopySurface* pCopySurface
#endif // RIO_IS_CAFE
);

#endif // FFLI_FACELINE_TEXTURE_H_
