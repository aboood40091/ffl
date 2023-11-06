#ifndef FFLI_MASK_TEXTURES_H_
#define FFLI_MASK_TEXTURES_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

struct FFLiRenderTexture;

struct FFLiMaskTextures
{
    FFLiRenderTexture*  pRenderTextures[FFL_EXPRESSION_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiMaskTextures) == 0x4C);

struct  FFLiCharInfo;
class   FFLiCopySurface;
struct  FFLiMaskTexturesTempObject;
struct  FFLiRenderTextureBuffer;
class   FFLiResourceLoader;
class   FFLiResourceManager;
class   FFLiShaderCallback;

FFLExpression FFLiInitMaskTextures(FFLiMaskTextures* pMaskTextures, u32 expressionFlag, u32 resolution, bool enableMipMap);
void FFLiDeleteMaskTextures(FFLiMaskTextures* pMaskTextures);

FFLResult FFLiInitTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, const FFLiMaskTextures* pMaskTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiRenderTextureBuffer* pRenderTextureBuffer);
void FFLiDeleteTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, u32 expressionFlag, FFLResourceType resourceType);

void FFLiRenderMaskTextures(FFLiMaskTextures* pMaskTextures, FFLiMaskTexturesTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface);

#endif // FFLI_MASK_TEXTURES_H_
