#ifndef FFLI_FACELINE_TEXTURE_H_
#define FFLI_FACELINE_TEXTURE_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

struct  FFLiCharInfo;
class   FFLiCopySurface;
struct  FFLiFacelineTextureTempObject;
class   FFLiRenderTexture;
struct  FFLiRenderTextureBuffer;
class   FFLiResourceLoader;
class   FFLiResourceManager;
class   FFLiShaderCallback;

void FFLiInitFacelineTexture(FFLiRenderTexture* pRenderTexture, u32 resolution, bool enableMipMap);
FFLResult FFLiInitTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiRenderTextureBuffer* pRenderTextureBuffer);

void FFLiRenderFacelineTexture(FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, FFLiFacelineTextureTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface);

#endif // FFLI_FACELINE_TEXTURE_H_
