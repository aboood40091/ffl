#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiPartsTextures.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

#if RIO_IS_CAFE
#include <gx2/mem.h>
#endif // RIO_IS_CAFE

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

void DeleteTextures_Eye(FFLiPartsTextures* pPartsTextures, u32 expressionFlag, bool isExpand);
void DeleteTextures_Mouth(FFLiPartsTextures* pPartsTextures, u32 expressionFlag, bool isExpand);
void DeleteTexture_Eyebrow(FFLiPartsTextures* pPartsTextures, bool isExpand);
void DeleteTexture_Mustache(FFLiPartsTextures* pPartsTextures, bool isExpand);
void DeleteTexture_Mole(FFLiPartsTextures* pPartsTextures, bool isExpand);

#if RIO_IS_CAFE
void InvalidateTexture(const GX2Texture& texture);
#endif // RIO_IS_CAFE

void InvalidateTextures(rio::Texture2D** ppTextures, u32 count);

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

FFLResult FFLiLoadPartsTextures(FFLiPartsTextures* pPartsTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, FFLiResourceLoader* pResLoader)
{
    rio::MemUtil::set(pPartsTextures, 0, sizeof(FFLiPartsTextures));

    bool useFlag[FFLI_EYE_TEXTURE_TYPE_MAX];    // max(FFLI_EYE_TEXTURE_TYPE_MAX, FFLI_MOUTH_TEXTURE_TYPE_MAX)

    {
        ExpressionToEyeUseFlag(useFlag, expressionFlag);

        for (u32 i = 0; i < FFLI_EYE_TEXTURE_TYPE_MAX; i++)
        {
            if (useFlag[i])
            {
                FFLResult result = FFLiLoadTextureWithAllocate(&(pPartsTextures->pTexturesEye[i]), FFLI_TEXTURE_PARTS_TYPE_EYE, FFLiCharInfoAndTypeToEyeIndex(pCharInfo, FFLiEyeTextureType(i)), pResLoader);
                if (result != FFL_RESULT_OK)
                {
                    for (u32 j = i; j > 0; j--)
                        if (useFlag[j - 1])
                            FFLiDeleteTexture(&(pPartsTextures->pTexturesEye[j - 1]), pResLoader->IsExpand());

                    return result;
                }
            }
        }
    }

    {
        ExpressionToMouthUseFlag(useFlag, expressionFlag);

        for (u32 i = 0; i < FFLI_MOUTH_TEXTURE_TYPE_MAX; i++)
        {
            if (useFlag[i])
            {
                FFLResult result = FFLiLoadTextureWithAllocate(&(pPartsTextures->pTexturesMouth[i]), FFLI_TEXTURE_PARTS_TYPE_MOUTH, FFLiCharInfoAndTypeToMouthIndex(pCharInfo, FFLiMouthTextureType(i)), pResLoader);
                if (result != FFL_RESULT_OK)
                {
                    for (u32 j = i; j > 0; j--)
                        if (useFlag[j - 1])
                            FFLiDeleteTexture(&(pPartsTextures->pTexturesMouth[j - 1]), pResLoader->IsExpand());

                    DeleteTextures_Eye(pPartsTextures, expressionFlag, pResLoader->IsExpand());
                    return result;
                }
            }
        }
    }

    FFLResult result = FFLiLoadTextureWithAllocate(&pPartsTextures->pTextureEyebrow, FFLI_TEXTURE_PARTS_TYPE_EYEBROW, pCharInfo->parts.eyebrowType, pResLoader);
    if (result != FFL_RESULT_OK)
    {
        DeleteTextures_Mouth(pPartsTextures, expressionFlag, pResLoader->IsExpand());
        DeleteTextures_Eye(pPartsTextures, expressionFlag, pResLoader->IsExpand());
        return result;
    }

    result = FFLiLoadTextureWithAllocate(&pPartsTextures->pTextureMustache, FFLI_TEXTURE_PARTS_TYPE_MUSTACHE, pCharInfo->parts.mustacheType, pResLoader);
    if (result != FFL_RESULT_OK)
    {
        DeleteTexture_Eyebrow(pPartsTextures, pResLoader->IsExpand());
        DeleteTextures_Mouth(pPartsTextures, expressionFlag, pResLoader->IsExpand());
        DeleteTextures_Eye(pPartsTextures, expressionFlag, pResLoader->IsExpand());
        return result;
    }

    result = FFLiLoadTextureWithAllocate(&pPartsTextures->pTextureMole, FFLI_TEXTURE_PARTS_TYPE_MOLE, pCharInfo->parts.moleType, pResLoader);
    if (result != FFL_RESULT_OK)
    {
        DeleteTexture_Mustache(pPartsTextures, pResLoader->IsExpand());
        DeleteTexture_Eyebrow(pPartsTextures, pResLoader->IsExpand());
        DeleteTextures_Mouth(pPartsTextures, expressionFlag, pResLoader->IsExpand());
        DeleteTextures_Eye(pPartsTextures, expressionFlag, pResLoader->IsExpand());
        return result;
    }

    return FFL_RESULT_OK;
}

void FFLiDeletePartsTextures(FFLiPartsTextures* pPartsTextures, u32 expressionFlag, FFLResourceType resourceType)
{
    RIO_ASSERT(FFLiManager::IsConstruct());
    bool isExpand = FFLiManager::GetInstance()->GetResourceManager().IsExpand(resourceType);

    DeleteTexture_Mole(pPartsTextures, isExpand);
    DeleteTexture_Mustache(pPartsTextures, isExpand);
    DeleteTexture_Eyebrow(pPartsTextures, isExpand);
    DeleteTextures_Mouth(pPartsTextures, expressionFlag, isExpand);
    DeleteTextures_Eye(pPartsTextures, expressionFlag, isExpand);
}

void FFLiInvalidatePartsTextures(FFLiPartsTextures* pPartsTextures)
{
    InvalidateTextures(pPartsTextures->pTexturesEye, FFLI_EYE_TEXTURE_TYPE_MAX);
    InvalidateTextures(pPartsTextures->pTexturesMouth, FFLI_MOUTH_TEXTURE_TYPE_MAX);

#if RIO_IS_CAFE
    if (pPartsTextures->pTextureEyebrow != NULL)
        InvalidateTexture(pPartsTextures->pTextureEyebrow->getNativeTexture());

    if (pPartsTextures->pTextureMustache != NULL)
        InvalidateTexture(pPartsTextures->pTextureMustache->getNativeTexture());

    if (pPartsTextures->pTextureMole != NULL)
        InvalidateTexture(pPartsTextures->pTextureMole->getNativeTexture());
#endif // RIO_IS_CAFE
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
    rio::MemUtil::set(pUseFlag, 0, sizeof(bool) * FFLI_EYE_TEXTURE_TYPE_MAX);

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
    rio::MemUtil::set(pUseFlag, 0, sizeof(bool) * FFLI_MOUTH_TEXTURE_TYPE_MAX);

    for (u32 i = 0; i < FFL_EXPRESSION_MAX; i++)
        if (expressionFlag & 1 << i)
            pUseFlag[EYE_MOUTH_TYPE_ELEMENT[i].mouthTextureType] = true;

}

void DeleteTextures_Eye(FFLiPartsTextures* pPartsTextures, u32 expressionFlag, bool isExpand)
{
    bool useFlag[FFLI_EYE_TEXTURE_TYPE_MAX];
    ExpressionToEyeUseFlag(useFlag, expressionFlag);

    for (u32 j = FFLI_EYE_TEXTURE_TYPE_MAX; j > 0; j--)
        if (useFlag[j - 1])
            FFLiDeleteTexture(&(pPartsTextures->pTexturesEye[j - 1]), isExpand);
}

void DeleteTextures_Mouth(FFLiPartsTextures* pPartsTextures, u32 expressionFlag, bool isExpand)
{
    bool useFlag[FFLI_MOUTH_TEXTURE_TYPE_MAX];
    ExpressionToMouthUseFlag(useFlag, expressionFlag);

    for (u32 j = FFLI_MOUTH_TEXTURE_TYPE_MAX; j > 0; j--)
        if (useFlag[j - 1])
            FFLiDeleteTexture(&(pPartsTextures->pTexturesMouth[j - 1]), isExpand);
}

void DeleteTexture_Eyebrow(FFLiPartsTextures* pPartsTextures, bool isExpand)
{
    FFLiDeleteTexture(&pPartsTextures->pTextureEyebrow, isExpand);
}

void DeleteTexture_Mustache(FFLiPartsTextures* pPartsTextures, bool isExpand)
{
    FFLiDeleteTexture(&pPartsTextures->pTextureMustache, isExpand);
}

void DeleteTexture_Mole(FFLiPartsTextures* pPartsTextures, bool isExpand)
{
    FFLiDeleteTexture(&pPartsTextures->pTextureMole, isExpand);
}

#if RIO_IS_CAFE

void InvalidateTexture(const GX2Texture& texture)
{
    if (texture.surface.image)
        GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, texture.surface.image, texture.surface.imageSize);

    if (texture.surface.mipmaps)
        GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, texture.surface.mipmaps, texture.surface.mipmapSize);
}

#endif // RIO_IS_CAFE

void InvalidateTextures(rio::Texture2D** ppTextures, u32 count)
{
#if RIO_IS_CAFE
    for (u32 i = 0; i < count; i++)
        if (ppTextures[i] != NULL)
            InvalidateTexture(ppTextures[i]->getNativeTexture());
#endif // RIO_IS_CAFE
}

}
