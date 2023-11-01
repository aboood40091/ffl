#include <nn/ffl/FFLiPartsTextures.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

#include <cstring>

namespace {

static const FFLiEyeMouthTypeElement EYE_MOUTH_TYPE_ELEMENT[FFL_EXPRESSION_MAX] = {
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_0 },
    { { FFLI_EYE_TEXTURE_TYPE_1, FFLI_EYE_TEXTURE_TYPE_1 }, FFLI_MOUTH_TEXTURE_TYPE_0 },
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_1 },
    { { FFLI_EYE_TEXTURE_TYPE_2, FFLI_EYE_TEXTURE_TYPE_2 }, FFLI_MOUTH_TEXTURE_TYPE_2 },
    { { FFLI_EYE_TEXTURE_TYPE_3, FFLI_EYE_TEXTURE_TYPE_3 }, FFLI_MOUTH_TEXTURE_TYPE_0 },
    { { FFLI_EYE_TEXTURE_TYPE_4, FFLI_EYE_TEXTURE_TYPE_4 }, FFLI_MOUTH_TEXTURE_TYPE_0 },
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_1, FFLI_EYE_TEXTURE_TYPE_1 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_2, FFLI_EYE_TEXTURE_TYPE_2 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_3, FFLI_EYE_TEXTURE_TYPE_3 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_4, FFLI_EYE_TEXTURE_TYPE_4 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_5, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_0 },
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_5 }, FFLI_MOUTH_TEXTURE_TYPE_0 },
    { { FFLI_EYE_TEXTURE_TYPE_5, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_5 }, FFLI_MOUTH_TEXTURE_TYPE_3 },
    { { FFLI_EYE_TEXTURE_TYPE_5, FFLI_EYE_TEXTURE_TYPE_0 }, FFLI_MOUTH_TEXTURE_TYPE_4 },
    { { FFLI_EYE_TEXTURE_TYPE_0, FFLI_EYE_TEXTURE_TYPE_5 }, FFLI_MOUTH_TEXTURE_TYPE_4 },
    { { FFLI_EYE_TEXTURE_TYPE_5, FFLI_EYE_TEXTURE_TYPE_5 }, FFLI_MOUTH_TEXTURE_TYPE_2 }
};

void ExpressionToEyeUseFlag(bool* pUseFlag, u32 expressionFlag);
void ExpressionToMouthUseFlag(bool* pUseFlag, u32 expressionFlag);

void InvalidateTextures(agl::TextureData** ppTextures, u32 count);

}

s32 FFLiCharInfoAndTypeToEyeIndex(const FFLiCharInfo* pCharInfo, FFLiEyeTextureType type)
{
    switch (type)
    {
    case FFLI_EYE_TEXTURE_TYPE_0:
    case FFLI_EYE_TEXTURE_TYPE_2:
        return pCharInfo->parts.eyeType;
    case FFLI_EYE_TEXTURE_TYPE_1:
        return 60;
    case FFLI_EYE_TEXTURE_TYPE_3:
        return 61;
    case FFLI_EYE_TEXTURE_TYPE_4:
        return 26;
    case FFLI_EYE_TEXTURE_TYPE_5:
        return 47;
    default:
        return 0;
    }
}

s32 FFLiCharInfoAndTypeToMouthIndex(const FFLiCharInfo* pCharInfo, FFLiMouthTextureType type)
{
    switch (type)
    {
    case FFLI_MOUTH_TEXTURE_TYPE_0:
        return pCharInfo->parts.eyeType;
    case FFLI_MOUTH_TEXTURE_TYPE_1:
        return 10;
    case FFLI_MOUTH_TEXTURE_TYPE_2:
        return 12;
    case FFLI_MOUTH_TEXTURE_TYPE_3:
        return 36;
    case FFLI_MOUTH_TEXTURE_TYPE_4:
        return 19;
    default:
        return 0;
    }
}

FFLResult FFLiLoadPartsTextures(FFLiPartsTextures* pPartsTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator)
{
    std::memset(pPartsTextures, 0, sizeof(FFLiPartsTextures));

    bool useFlag[FFLI_EYE_TEXTURE_TYPE_MAX];    // max(FFLI_EYE_TEXTURE_TYPE_MAX, FFLI_MOUTH_TEXTURE_TYPE_MAX)

    {
        ExpressionToEyeUseFlag(useFlag, expressionFlag);

        for (u32 i = 0; i < FFLI_EYE_TEXTURE_TYPE_MAX; i++)
        {
            if (useFlag[i])
            {
                FFLResult result = FFLiLoadTextureWithAllocate(&(pPartsTextures->pTexturesEye[i]), FFLI_TEXTURE_PARTS_TYPE_EYE, FFLiCharInfoAndTypeToEyeIndex(pCharInfo, FFLiEyeTextureType(i)), pResLoader, pAllocator);
                if (result != FFL_RESULT_OK)
                    return result;
            }
        }
    }

    {
        ExpressionToMouthUseFlag(useFlag, expressionFlag);

        for (u32 i = 0; i < FFLI_MOUTH_TEXTURE_TYPE_MAX; i++)
        {
            if (useFlag[i])
            {
                FFLResult result = FFLiLoadTextureWithAllocate(&(pPartsTextures->pTexturesMouth[i]), FFLI_TEXTURE_PARTS_TYPE_MOUTH, FFLiCharInfoAndTypeToMouthIndex(pCharInfo, FFLiMouthTextureType(i)), pResLoader, pAllocator);
                if (result != FFL_RESULT_OK)
                    return result;
            }
        }
    }

    FFLResult result = FFLiLoadTextureWithAllocate(&pPartsTextures->pTextureEyebrow, FFLI_TEXTURE_PARTS_TYPE_EYEBROW, pCharInfo->parts.eyebrowType, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    result = FFLiLoadTextureWithAllocate(&pPartsTextures->pTextureMustache, FFLI_TEXTURE_PARTS_TYPE_MUSTACHE, pCharInfo->parts.mustacheType, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    result = FFLiLoadTextureWithAllocate(&pPartsTextures->pTextureMole, FFLI_TEXTURE_PARTS_TYPE_MOLE, pCharInfo->parts.moleType, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    return FFL_RESULT_OK;
}

void FFLiInvalidatePartsTextures(FFLiPartsTextures* pPartsTextures)
{
    InvalidateTextures(pPartsTextures->pTexturesEye, FFLI_EYE_TEXTURE_TYPE_MAX);
    InvalidateTextures(pPartsTextures->pTexturesMouth, FFLI_MOUTH_TEXTURE_TYPE_MAX);

    if (pPartsTextures->pTextureEyebrow != NULL)
        pPartsTextures->pTextureEyebrow->invalidateGPUCache();

    if (pPartsTextures->pTextureMustache != NULL)
        pPartsTextures->pTextureMustache->invalidateGPUCache();

    if (pPartsTextures->pTextureMole != NULL)
        pPartsTextures->pTextureMole->invalidateGPUCache();
}

const FFLiEyeMouthTypeElement& FFLiGetEyeMouthTypeElement(FFLExpression expression)
{
    return EYE_MOUTH_TYPE_ELEMENT[expression];
}

u32 FFLiGetMaxMouthNum(u32 expressionFlagCount)
{
    return FFLiMin<u32>(expressionFlagCount, FFLI_MOUTH_TEXTURE_TYPE_MAX);
}

u32 FFLiGetMaxEyeNum(u32 expressionFlagCount)
{
    return FFLiMin<u32>(expressionFlagCount + 1, FFLI_EYE_TEXTURE_TYPE_MAX);
}

namespace {

void ExpressionToEyeUseFlag(bool* pUseFlag, u32 expressionFlag)
{
    std::memset(pUseFlag, 0, sizeof(bool) * FFLI_EYE_TEXTURE_TYPE_MAX);

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
    {
        if (expressionFlag & 1 << i)
        {
            pUseFlag[EYE_MOUTH_TYPE_ELEMENT[i].eyeTextureType[0]] = true;
            pUseFlag[EYE_MOUTH_TYPE_ELEMENT[i].eyeTextureType[1]] = true;
        }
    }
}

void ExpressionToMouthUseFlag(bool* pUseFlag, u32 expressionFlag)
{
    std::memset(pUseFlag, 0, sizeof(bool) * FFLI_MOUTH_TEXTURE_TYPE_MAX);

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
        if (expressionFlag & 1 << i)
            pUseFlag[EYE_MOUTH_TYPE_ELEMENT[i].mouthTextureType] = true;

}

void InvalidateTextures(agl::TextureData** ppTextures, u32 count)
{
    for (u32 i = 0; i < count; i++)
        if (ppTextures[i] != NULL)
            ppTextures[i]->invalidateGPUCache();
}

}
