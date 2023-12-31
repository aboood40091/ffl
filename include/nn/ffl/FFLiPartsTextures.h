#ifndef FFLI_PARTS_TEXTURES_H_
#define FFLI_PARTS_TEXTURES_H_

#include <nn/ffl/FFLExpression.h>
#include <nn/ffl/FFLResult.h>

#include <cafe/gx2.h>

enum FFLiEyeTextureType
{
    FFLI_EYE_TEXTURE_TYPE_0     = 0,
    FFLI_EYE_TEXTURE_TYPE_1     = 1,
    FFLI_EYE_TEXTURE_TYPE_2     = 2,
    FFLI_EYE_TEXTURE_TYPE_3     = 3,
    FFLI_EYE_TEXTURE_TYPE_4     = 4,
    FFLI_EYE_TEXTURE_TYPE_5     = 5,
    FFLI_EYE_TEXTURE_TYPE_MAX   = 6
};

enum FFLiMouthTextureType
{
    FFLI_MOUTH_TEXTURE_TYPE_0   = 0,
    FFLI_MOUTH_TEXTURE_TYPE_1   = 1,
    FFLI_MOUTH_TEXTURE_TYPE_2   = 2,
    FFLI_MOUTH_TEXTURE_TYPE_3   = 3,
    FFLI_MOUTH_TEXTURE_TYPE_4   = 4,
    FFLI_MOUTH_TEXTURE_TYPE_MAX = 5
};

struct FFLiPartsTextures
{
    GX2Texture* pTexturesEye[FFLI_EYE_TEXTURE_TYPE_MAX];
    GX2Texture* pTexturesMouth[FFLI_MOUTH_TEXTURE_TYPE_MAX];
    GX2Texture* pTextureEyebrow;
    GX2Texture* pTextureMustache;
    GX2Texture* pTextureMole;
};
NN_STATIC_ASSERT(sizeof(FFLiPartsTextures) == 0x38);

struct FFLiEyeMouthTypeElement
{
    FFLiEyeTextureType      eyeTextureType[2];
    FFLiMouthTextureType    mouthTextureType;
};
NN_STATIC_ASSERT(sizeof(FFLiEyeMouthTypeElement) == 0xC);

class   FFLiBufferAllocator;
struct  FFLiCharInfo;
class   FFLiResourceLoader;

s32 FFLiCharInfoAndTypeToEyeIndex(const FFLiCharInfo* pCharInfo, FFLiEyeTextureType type);
s32 FFLiCharInfoAndTypeToMouthIndex(const FFLiCharInfo* pCharInfo, FFLiMouthTextureType type);

FFLResult FFLiLoadPartsTextures(FFLiPartsTextures* pPartsTextures, const FFLiCharInfo* pCharInfo, u32 expressionFlag, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator);

void FFLiInvalidatePartsTextures(FFLiPartsTextures* pPartsTextures);

const FFLiEyeMouthTypeElement& FFLiGetEyeMouthTypeElement(FFLExpression expression);

u32 FFLiGetMaxMouthNum(u32 expressionFlagCount);
u32 FFLiGetMaxEyeNum(u32 expressionFlagCount);

#endif // FFLI_PARTS_TEXTURES_H_
