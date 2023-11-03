#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiMaskTextures.h>
#include <nn/ffl/FFLiMaskTexturesTempObject.h>
#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiPartsTextures.h>
#include <nn/ffl/FFLiRawMask.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>
#include <nn/ffl/detail/FFLiCopySurface.h>

#include <cstring>

namespace {

rio::TextureFormat GetTextureFormat(bool useOffScreenSrgbFetch);

FFLiRenderTexture* FFLiRenderTextureAllocate();

bool CanUseExpression(u32 expressionFlag, FFLExpression expression);

void InitRawMask(FFLiMaskTexturesTempObject* pObject, u32 expressionFlag);

void SetupExpressionCharInfo(FFLiCharInfo* pExpressionCharInfo, const FFLiCharInfo* pCharInfo, FFLExpression expression);

}

FFLExpression FFLiInitMaskTextures(FFLiMaskTextures* pMaskTextures, u32 expressionFlag, u32 resolution, bool enableMipMap)
{
    FFLExpression expression = FFL_EXPRESSION_MAX;

    u32 numMips = enableMipMap ? FFLiGetMipMapNum(resolution, resolution) : 1;

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
    {
        if ((expressionFlag & 1 << i) == 0)
        {
            pMaskTextures->pRenderTextures[i] = NULL;
            continue;
        }

        if (expression == FFL_EXPRESSION_MAX)
            expression = FFLExpression(i);

        pMaskTextures->pRenderTextures[i] = FFLiRenderTextureAllocate();
        rio::TextureFormat format = GetTextureFormat(FFLiUseOffScreenSrgbFetch());
        FFLiInitRenderTexture(pMaskTextures->pRenderTextures[i], resolution, resolution, format, numMips);
    }

    return expression;
}

FFLResult FFLiInitTempObjectMaskTextures(FFLiMaskTexturesTempObject* pObject, const FFLiMaskTextures* pMaskTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiRenderTextureBuffer* pRenderTextureBuffer)
{
    std::memset(pObject, 0, sizeof(FFLiMaskTexturesTempObject));

    FFLResult result = FFLiLoadPartsTextures(&pObject->partsTextures, pCharInfo, expressionFlag, pResLoader);
    if (result != FFL_RESULT_OK)
        return result;

    InitRawMask(pObject, expressionFlag);

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
                &desc
            );
        }
    }

    return FFL_RESULT_OK;
}

void FFLiRenderMaskTextures(FFLiMaskTextures* pMaskTextures, FFLiMaskTexturesTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface)
{
    static const FFLColor BLACK = { };

    FFLiInvalidatePartsTextures(&pObject->partsTextures);

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
    {
        if (pMaskTextures->pRenderTextures[i] != NULL && pObject->pRawMaskDrawParam[i] != NULL)
        {
            FFLiInvalidateRawMask(pObject->pRawMaskDrawParam[i]);

            FFLiRenderTexture& renderTexture = *(pMaskTextures->pRenderTextures[i]);

            FFLiInvalidateRenderTexture(&renderTexture);
            RIO_ASSERT(renderTexture.textureData.getTextureFormat() == agl::cTextureFormat_R8_G8_B8_A8_uNorm);
            FFLiSetupRenderTexture(&renderTexture, &BLACK, NULL, 0, pCallback);

            FFLiDrawRawMask(pObject->pRawMaskDrawParam[i], pCallback);

            if (renderTexture.textureData.getMipLevelNum() > 1)
            {
                pCopySurface->Begin();
                for (u32 i = 1; i < renderTexture.textureData.getMipLevelNum(); i++)
                    pCopySurface->Execute(&renderTexture.textureData, i, i - 1);
            }

            pCallback->CallSetContextState();

            FFLiFlushRenderTexture(&renderTexture);
        }
    }
}

namespace {

rio::TextureFormat GetTextureFormat(bool useOffScreenSrgbFetch)
{
    if (useOffScreenSrgbFetch)
        return rio::TEXTURE_FORMAT_R8_G8_B8_A8_SRGB;

    else
        return rio::TEXTURE_FORMAT_R8_G8_B8_A8_UNORM;
}

FFLiRenderTexture* FFLiRenderTextureAllocate()
{
    return new FFLiRenderTexture;
}

bool CanUseExpression(u32 expressionFlag, FFLExpression expression)
{
    return (expressionFlag & 1 << expression) != 0;
}

void InitRawMask(FFLiMaskTexturesTempObject* pObject, u32 expressionFlag)
{
    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
        if (CanUseExpression(expressionFlag, FFLExpression(i)))
            pObject->pRawMaskDrawParam[i] = new FFLiRawMaskDrawParam;
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

}
