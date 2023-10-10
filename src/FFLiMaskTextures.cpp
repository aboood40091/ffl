#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiMaskTextures.h>
#include <nn/ffl/FFLiMaskTexturesTempObject.h>
#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiPartsTextures.h>
#include <nn/ffl/FFLiRawMask.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>
#include <nn/ffl/detail/FFLiCompressorImpl.h>
#include <nn/ffl/detail/FFLiCopySurface.h>

#include <cstring>

namespace {

u32 ExpressionFlagCount(u32 expressionFlag);

u32 GetTextureBufferSize(u32 resolution, u32 numMips, bool compressTexture);

GX2SurfaceFormat GetTextureFormat(bool compressTexture, bool useOffScreenSrgbFetch);

FFLiRenderTexture* FFLiRenderTextureAllocate(FFLiBufferAllocator* pAllocator);

bool CanUseExpression(u32 expressionFlag, FFLExpression expression);

void InitRawMask(FFLiMaskTexturesTempObject* pObject, u32 expressionFlag, FFLiBufferAllocator* pAllocator);

void SetupExpressionCharInfo(FFLiCharInfo* pExpressionCharInfo, const FFLiCharInfo* pCharInfo, FFLExpression expression);

void Compress(FFLiCompressorImplBC3* pCompressorImpl, FFLiRenderTexture* pDst, const FFLiRenderTexture* pSrc, const FFLiCompressorParam* pParam);

}

u32 FFLiGetBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, bool compressTexture)
{
    u32 count = ExpressionFlagCount(expressionFlag);

    u32 numMips = enableMipMap ? FFLiGetMipMapNum(resolution, resolution) : 1;

    u32 ret  = FFLiRoundUp(GetTextureBufferSize(resolution, numMips, compressTexture) * count, 4);
    ret     += FFLiRoundUp(sizeof(FFLiRenderTexture), 4) * count;

    return ret;
}

u32 FFLiGetTempBufferSizeMaskTextures(u32 expressionFlag, u32 resolution, bool enableMipMap, bool compressTexture, FFLiResourceManager* pResourceManager, FFLResourceType resourceType)
{
    u32 count = ExpressionFlagCount(expressionFlag);
    u32 eyeCount = FFLiGetMaxEyeNum(count);
    u32 mouthCount = FFLiGetMaxMouthNum(count);

    u32 numMips = enableMipMap ? FFLiGetMipMapNum(resolution, resolution) : 1;

    u32 ret  = FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_EYE) * eyeCount;
    ret     += FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_MOUTH) * mouthCount;
    ret     += FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_EYEBROW);
    ret     += FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_MOLE);
    ret     += FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_MUSTACHE);

    ret     += sizeof(FFLiRawMaskDrawParam) * count;
    ret     += FFLiGetBufferRawMask() * count;

    if (compressTexture)
    {
        ret += FFLiRoundUp(sizeof(FFLiRenderTexture), 4);

        ret += sizeof(FFLiCompressorParam) * count;
        ret += FFLiCompressorParam::GetBufferSize(numMips) * count;
    }

    return ret;
}

u32 FFLiGetCompressBufferSizeMaskTexture(u32 resolution, bool enableMipMap)
{
    u32 numMips = enableMipMap ? FFLiGetMipMapNum(resolution, resolution) : 1;
    return GetTextureBufferSize(resolution, numMips, false); // Did they mean to put true here?
}

FFLExpression FFLiInitMaskTextures(FFLiMaskTextures* pMaskTextures, u32 expressionFlag, u32 resolution, bool enableMipMap, bool compressTexture, FFLiBufferAllocator* pAllocator)
{
    FFLExpression expression = FFL_EXPRESSION_MAX;

    u32 numMips = enableMipMap ? FFLiGetMipMapNum(resolution, resolution) : 1;

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
    {
        if ((expressionFlag & 1 << (i & 0x3f)) == 0)
        {
            pMaskTextures->pRenderTextures[i] = NULL;
            continue;
        }

        if (expression == FFL_EXPRESSION_MAX)
            expression = FFLExpression(i);

        pMaskTextures->pRenderTextures[i] = FFLiRenderTextureAllocate(pAllocator);
        GX2SurfaceFormat format = GetTextureFormat(compressTexture, FFLiUseOffScreenSrgbFetch());
        FFLiInitRenderTexture(pMaskTextures->pRenderTextures[i], resolution, resolution, format, numMips, pAllocator);
    }

    return expression;
}

FFLResult FFLiInitTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, const FFLiMaskTextures* pMaskTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, u32 resolution, bool enableMipMap, bool compressTexture, bool compressUseUB, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, FFLiRenderTextureBuffer* pRenderTextureBuffer)
{
    std::memset(pObject, 0, sizeof(FFLiMaskTexturesTempObject));

    FFLResult result = FFLiLoadPartsTextures(&pObject->partsTextures, pCharInfo, expressionFlag, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    InitRawMask(pObject, expressionFlag, pAllocator);

    u32 numMips = enableMipMap ? FFLiGetMipMapNum(resolution, resolution) : 1;

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
    {
        if (CanUseExpression(expressionFlag, FFLExpression(i)))
        {
            FFLiRawMaskTextureDesc desc;
            FFLiCharInfo expressionCharInfo = *pCharInfo;
            const FFLiEyeMouthTypeElement& element = FFLiGetEyeMouthTypeElement(FFLExpression(i));

            SetupExpressionCharInfo(&expressionCharInfo, pCharInfo, FFLExpression(i));

            desc.pTexturesEye[0] = pObject->partsTextures.pTexturesEye[element.eyeTextureType[0]];
            desc.pTexturesEye[1] = pObject->partsTextures.pTexturesEye[element.eyeTextureType[1]];

            desc.pTexturesEyebrow[0] = pObject->partsTextures.pTextureEyebrow;
            desc.pTexturesEyebrow[1] = pObject->partsTextures.pTextureEyebrow;

            desc.pTextureMouth = pObject->partsTextures.pTexturesMouth[element.mouthTextureType];

            desc.pTexturesMustache[0] = pObject->partsTextures.pTextureMustache;
            desc.pTexturesMustache[1] = pObject->partsTextures.pTextureMustache;

            desc.pTextureMole = pObject->partsTextures.pTextureMole;

            FFLiInitDrawParamRawMask(
                pObject->pRawMaskDrawParam[i],
                &expressionCharInfo,
                resolution,
                FFLiCharInfoAndTypeToEyeIndex(pCharInfo, element.eyeTextureType[0]),
                FFLiCharInfoAndTypeToEyeIndex(pCharInfo, element.eyeTextureType[1]),
                &desc,
                pAllocator
            );

            if (compressTexture)
                pObject->pCompressorParam[i] = FFLiCompressorParam::Create(numMips, compressUseUB, pAllocator);
        }
    }

    if (compressTexture)
    {
        pObject->pRenderTexture = FFLiRenderTextureAllocate(pAllocator);
        FFLiInitByBufferRenderTexture(pObject->pRenderTexture, resolution, resolution, GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM, numMips, pRenderTextureBuffer);
    }

    return FFL_RESULT_OK;
}

void FFLiRenderMaskTextures(FFLiMaskTextures* pMaskTextures, FFLiMaskTexturesTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface, FFLiCompressorImplBC3* pCompressorImpl)
{
    static const FFLColor BLACK = { };

    FFLiInvalidatePartsTextures(&pObject->partsTextures);

    bool useTempRenderTexture = pObject->pRenderTexture != NULL;

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
    {
        if (pMaskTextures->pRenderTextures[i] != NULL && pObject->pRawMaskDrawParam[i] != NULL)
        {
            FFLiInvalidateRawMask(pObject->pRawMaskDrawParam[i]);

            FFLiRenderTexture& renderTexture = *(useTempRenderTexture ? pObject->pRenderTexture : pMaskTextures->pRenderTextures[i]);

            FFLiInvalidateRenderTexture(&renderTexture);
            FFLiSetupRenderTexture(&renderTexture, &BLACK, NULL, 0, GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM, pCallback);

            FFLiDrawRawMask(pObject->pRawMaskDrawParam[i], pCallback);

            if (renderTexture.gx2Texture.surface.numMips > 1)
            {
                pCopySurface->Begin();
                for (u32 i = 1; i < renderTexture.gx2Texture.surface.numMips; i++)
                    pCopySurface->Execute(&renderTexture.gx2Texture.surface, i, &renderTexture.gx2Texture.surface, i - 1);
            }

            pCallback->CallSetContextState();

            FFLiFlushRenderTexture(&renderTexture);

            if (useTempRenderTexture)
            {
                const FFLiRenderTexture* pSrc = &renderTexture;
                FFLiRenderTexture* pDst = pMaskTextures->pRenderTextures[i];
                FFLiCompressorParam* pParam = pObject->pCompressorParam[i];
                
                FFLiInvalidateTexture(&pDst->gx2Texture);
                pParam->SetTexture(&pSrc->gx2Texture);
            
                Compress(pCompressorImpl, pDst, pSrc, pParam);

                FFLiFlushRenderTexture(pDst);

                pCallback->CallSetContextState();
            }
        }
    }
}

namespace {

u32 ExpressionFlagCount(u32 expressionFlag)
{
    u32 ret = 0;

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
        if (expressionFlag & 1 << (i & 0x3f))
            ret++;

    return ret;
}

u32 GetTextureBufferSize(u32 resolution, u32 numMips, bool compressTexture)
{
    u32 ret = 0;

    if (compressTexture)
        ret = FFLiCompressorImplBC3::GetTextureBufferSize(resolution, resolution, numMips);

    else
        ret = FFLiGetBufferRenderTexture(resolution, resolution, GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM, numMips);

    return ret;
}

GX2SurfaceFormat GetTextureFormat(bool compressTexture, bool useOffScreenSrgbFetch)
{
    if (compressTexture)
    {
        if (useOffScreenSrgbFetch)
            return GX2_SURFACE_FORMAT_T_BC3_SRGB;

        else
            return GX2_SURFACE_FORMAT_T_BC3_UNORM;
    }
    else
    {
        if (useOffScreenSrgbFetch)
            return GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_SRGB;

        else
            return GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM;
    }
}

FFLiRenderTexture* FFLiRenderTextureAllocate(FFLiBufferAllocator* pAllocator)
{
    return static_cast<FFLiRenderTexture*>(FFLiAllocateBufferAllocator(pAllocator, sizeof(FFLiRenderTexture), 4));
}

bool CanUseExpression(u32 expressionFlag, FFLExpression expression)
{
    return (expressionFlag & 1 << (expression & 0x3f)) != 0;
}

void InitRawMask(FFLiMaskTexturesTempObject* pObject, u32 expressionFlag, FFLiBufferAllocator* pAllocator)
{
    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
        if (CanUseExpression(expressionFlag, FFLExpression(i)))
            pObject->pRawMaskDrawParam[i] = static_cast<FFLiRawMaskDrawParam*>(FFLiAllocateBufferAllocator(pAllocator, sizeof(FFLiRawMaskDrawParam)));
}

struct CorrectParam
{
    s32 eyeType;
    s32 mouthType;
    s32 eyeRotateOffset;
    s32 eyebrowRotateOffset;
    s32 eyebrowPositionY;
};

static const CorrectParam CORRECT_PARAM[FFL_EXPRESSION_MAX] = {
/*
    {   -1,   -1,  0,  0,  0 },
    { 0x3C,   -1,  0,  0,  0 },
    {   -1,  0xA,  2,  2,  0 },
    {   -1,  0xC, -2, -2,  0 },
    { 0x3D,   -1,  0,  0, -2 },
    { 0x1A,   -1,  0,  0,  0 },
    {   -1, 0x24,  0,  0,  0 },
    { 0x3C, 0x24,  0,  0,  0 },
    {   -1, 0x24,  2,  2,  0 },
    {   -1, 0x24, -2, -2,  0 },
    { 0x3D, 0x24,  0,  0, -2 },
    { 0x1A, 0x24,  0,  0,  0 },
    { 0x2F,   -1,  0,  0,  0 },
    { 0x2F,   -1,  0,  0,  0 },
    { 0x2F, 0x24,  0,  0,  0 },
    { 0x2F, 0x24,  0,  0,  0 },
    { 0x2F,   -1,  0,  0,  0 },
    { 0x2F,   -1,  0,  0,  0 },
    { 0x2F,  0xC,  0,  0,  0 }
*/
    { -1, -1,  0,  0,  0 },
    { 60, -1,  0,  0,  0 },
    { -1, 10,  2,  2,  0 },
    { -1, 12, -2, -2,  0 },
    { 61, -1,  0,  0, -2 },
    { 26, -1,  0,  0,  0 },
    { -1, 36,  0,  0,  0 },
    { 60, 36,  0,  0,  0 },
    { -1, 36,  2,  2,  0 },
    { -1, 36, -2, -2,  0 },
    { 61, 36,  0,  0, -2 },
    { 26, 36,  0,  0,  0 },
    { 47, -1,  0,  0,  0 },
    { 47, -1,  0,  0,  0 },
    { 47, 36,  0,  0,  0 },
    { 47, 36,  0,  0,  0 },
    { 47, -1,  0,  0,  0 },
    { 47, -1,  0,  0,  0 },
    { 47, 12,  0,  0,  0 }
};

const CorrectParam& GetCorrectParam(FFLExpression expression)
{
    return CORRECT_PARAM[expression];
}

void SetupExpressionCharInfo(FFLiCharInfo* pExpressionCharInfo, const FFLiCharInfo*, FFLExpression expression)
{
    const CorrectParam& param = GetCorrectParam(expression);

    if (param.mouthType >= 0)
        pExpressionCharInfo->parts.mouthType = param.mouthType;

    s32 eyeRotateOffset = param.eyeRotateOffset;
    if (param.eyeType >= 0 && param.eyeType != pExpressionCharInfo->parts.eyeType)
    {
        eyeRotateOffset += FFLiiGetEyeRotateOffset(pExpressionCharInfo->parts.eyeType);
        eyeRotateOffset -= FFLiiGetEyeRotateOffset(param.eyeType);
    }

    if (eyeRotateOffset != 0)
    {
        // pExpressionCharInfo->parts.eyeRotate = clamp(pExpressionCharInfo->parts.eyeRotate + eyeRotateOffset, 0, 7);
        s32 eyeRotate = pExpressionCharInfo->parts.eyeRotate + eyeRotateOffset;
        if (eyeRotate < 0)
            eyeRotate = 0;
        else if (eyeRotate > 7)
            eyeRotate = 7;
        pExpressionCharInfo->parts.eyeRotate = eyeRotate;
    }

    pExpressionCharInfo->parts.eyebrowPositionY += param.eyebrowPositionY;

    s32 eyebrowRotateOffset = param.eyebrowRotateOffset;
    if (eyebrowRotateOffset != 0)
    {
        // pExpressionCharInfo->parts.eyebrowRotate = clamp(pExpressionCharInfo->parts.eyebrowRotate + eyebrowRotateOffset, 0, 11);
        s32 eyebrowRotate = pExpressionCharInfo->parts.eyebrowRotate + eyebrowRotateOffset;
        if (eyebrowRotate < 0)
            eyebrowRotate = 0;
        else if (eyebrowRotate > 11)
            eyebrowRotate = 11;
        pExpressionCharInfo->parts.eyebrowRotate = eyebrowRotate;
    }
}

void Compress(FFLiCompressorImplBC3* pCompressorImpl, FFLiRenderTexture* pDst, const FFLiRenderTexture* pSrc, const FFLiCompressorParam* pParam)
{
    pCompressorImpl->CompressImpl(&pDst->gx2Texture, &pSrc->gx2Texture, pParam);
}

}
