#ifndef FFLI_MASK_TEXTURES_H_
#define FFLI_MASK_TEXTURES_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <misc/rio_Types.h>

struct FFLiRenderTexture;

struct FFLiMaskTextures
{
    FFLiRenderTexture*  pRenderTextures[FFL_EXPRESSION_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiMaskTextures) == 0x4C);

struct  FFLiCharInfo;
#if RIO_IS_CAFE
class   FFLiCopySurface;
#endif // RIO_IS_CAFE
struct  FFLiMaskTexturesTempObject;
class   FFLiResourceLoader;
class   FFLiResourceManager;
class   FFLiShaderCallback;

FFLExpression FFLiInitMaskTextures(FFLiMaskTextures* pMaskTextures, u32 expressionFlag, u32 resolution, bool enableMipMap);
void FFLiDeleteMaskTextures(FFLiMaskTextures* pMaskTextures);

FFLResult FFLiInitTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, const FFLiMaskTextures* pMaskTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader);
void FFLiDeleteTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, u32 expressionFlag, FFLResourceType resourceType);

void FFLiRenderMaskTextures(FFLiMaskTextures* pMaskTextures, FFLiMaskTexturesTempObject* pObject, const FFLiShaderCallback* pCallback
#if RIO_IS_CAFE
    , FFLiCopySurface* pCopySurface
#endif // RIO_IS_CAFE
);

#endif // FFLI_MASK_TEXTURES_H_
