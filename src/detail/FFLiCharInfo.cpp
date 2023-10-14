#include <nn/ffl/FFLAdditionalInfo.h>

#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiMisc.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>

#include <cstring>

namespace {

bool VerifyName(const u16* s, s32 size);

bool CompareMiiVersion(const FFLiCharInfo* a, const FFLiCharInfo* b);
bool CompareCopyable(const FFLiCharInfo* a, const FFLiCharInfo* b);
bool CompareLocalOnly(const FFLiCharInfo* a, const FFLiCharInfo* b);
bool CompareRegionMove(const FFLiCharInfo* a, const FFLiCharInfo* b);
bool CompareBirthPlatform(const FFLiCharInfo* a, const FFLiCharInfo* b);

}

BOOL FFLiVerifyCharInfo(const FFLiCharInfo* pCharInfo, BOOL verifyName)
{
    return FFLiiVerifyCharInfo(pCharInfo, verifyName);
}

FFLiVerifyCharInfoReason FFLiVerifyCharInfoWithReason(const FFLiCharInfo* pCharInfo, BOOL verifyName)
{
    if (!FFLiRange(0, 11, pCharInfo->parts.faceType))
        return FFLI_VERIFY_CHAR_INFO_REASON_FACE_TYPE_INVALID;

    if (!FFLiRange(0, 5, pCharInfo->parts.facelineColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_FACELINE_COLOR_INVALID;

    if (!FFLiRange(0, 11, pCharInfo->parts.faceLine))
        return FFLI_VERIFY_CHAR_INFO_REASON_FACE_LINE_INVALID;

    if (!FFLiRange(0, 11, pCharInfo->parts.faceMakeup))
        return FFLI_VERIFY_CHAR_INFO_REASON_FACE_MAKE_INVALID;

    if (!FFLiRange(0, 131, pCharInfo->parts.hairType))
        return FFLI_VERIFY_CHAR_INFO_REASON_HAIR_TYPE_INVALID;

    if (!FFLiRange(0, 7, pCharInfo->parts.hairColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_HAIR_COLOR_INVALID;

    if (!FFLiRange(0, 1, pCharInfo->parts.hairDir))
        return FFLI_VERIFY_CHAR_INFO_REASON_HAIR_DIR_INVALID;

    if (!FFLiRange(0, 59, pCharInfo->parts.eyeType))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_TYPE_INVALID;

    if (!FFLiRange(0, 5, pCharInfo->parts.eyeColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_COLOR_INVALID;

    if (!FFLiRange(0, 7, pCharInfo->parts.eyeScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_SCALE_INVALID;

    if (!FFLiRange(0, 6, pCharInfo->parts.eyeScaleY))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_SCALE_Y_INVALID;

    if (!FFLiRange(0, 7, pCharInfo->parts.eyeRotate))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_ROTATE_INVALID;

    if (!FFLiRange(0, 12, pCharInfo->parts.eyeSpacingX))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_SPACING_INVALID;

    if (!FFLiRange(0, 18, pCharInfo->parts.eyePositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYE_POS_INVALID;

    if (!FFLiRange(0, 23, pCharInfo->parts.eyebrowType))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_TYPE_INVALID;

    if (!FFLiRange(0, 7, pCharInfo->parts.eyebrowColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_COLOR_INVALID;

    if (!FFLiRange(0, 8, pCharInfo->parts.eyebrowScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_SCALE_INVALID;

    if (!FFLiRange(0, 6, pCharInfo->parts.eyebrowScaleY))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_SCALE_Y_INVALID;

    if (!FFLiRange(0, 11, pCharInfo->parts.eyebrowRotate))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_ROTATE_INVALID;

    if (!FFLiRange(0, 12, pCharInfo->parts.eyebrowSpacingX))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_SPACING_INVALID;

    if (!FFLiRange(3, 18, pCharInfo->parts.eyebrowPositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_EYEBROW_POS_INVALID;

    if (!FFLiRange(0, 17, pCharInfo->parts.noseType))
        return FFLI_VERIFY_CHAR_INFO_REASON_NOSE_TYPE_INVALID;

    if (!FFLiRange(0, 8, pCharInfo->parts.noseScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_NOSE_SCALE_INVALID;

    if (!FFLiRange(0, 18, pCharInfo->parts.nosePositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_NOSE_POS_INVALID;

    if (!FFLiRange(0, 35, pCharInfo->parts.mouthType))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOUTH_TYPE_INVALID;

    if (!FFLiRange(0, 4, pCharInfo->parts.mouthColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOUTH_COLOR_INVALID;

    if (!FFLiRange(0, 8, pCharInfo->parts.mouthScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOUTH_SCALE_INVALID;

    if (!FFLiRange(0, 6, pCharInfo->parts.mouthScaleY))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOUTH_SCALE_Y_INVALID;

    if (!FFLiRange(0, 18, pCharInfo->parts.mouthPositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOUTH_POS_INVALID;

    if (!FFLiRange(0, 5, pCharInfo->parts.mustacheType))
        return FFLI_VERIFY_CHAR_INFO_REASON_MUSTACHE_TYPE_INVALID;

    if (!FFLiRange(0, 5, pCharInfo->parts.beardType))
        return FFLI_VERIFY_CHAR_INFO_REASON_BEARD_TYPE_INVALID;

    if (!FFLiRange(0, 7, pCharInfo->parts.beardColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_BEARD_COLOR_INVALID;

    if (!FFLiRange(0, 8, pCharInfo->parts.mustacheScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_MUSTACHE_SCALE_INVALID;

    if (!FFLiRange(0, 16, pCharInfo->parts.mustachePositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_MUSTACHE_POS_INVALID;

    if (!FFLiRange(0, 8, pCharInfo->parts.glassType))
        return FFLI_VERIFY_CHAR_INFO_REASON_GLASS_TYPE_INVALID;

    if (!FFLiRange(0, 5, pCharInfo->parts.glassColor))
        return FFLI_VERIFY_CHAR_INFO_REASON_GLASS_COLOR_INVALID;

    if (!FFLiRange(0, 7, pCharInfo->parts.glassScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_GLASS_SCALE_INVALID;

    if (!FFLiRange(0, 20, pCharInfo->parts.glassPositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_GLASS_POS_INVALID;

    if (!FFLiRange(0, 1, pCharInfo->parts.moleType))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOLE_TYPE_INVALID;

    if (!FFLiRange(0, 8, pCharInfo->parts.moleScale))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOLE_SCALE_INVALID;

    if (!FFLiRange(0, 16, pCharInfo->parts.molePositionX))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOLE_POS_X_INVALID;

    if (!FFLiRange(0, 30, pCharInfo->parts.molePositionY))
        return FFLI_VERIFY_CHAR_INFO_REASON_MOLE_POS_Y_INVALID;

    if (verifyName)
    {
        if (pCharInfo->name[0] == L'\0' || !VerifyName(pCharInfo->name, 10))
            return FFLI_VERIFY_CHAR_INFO_REASON_NAME_INVALID;

        if (!VerifyName(pCharInfo->creatorName, 10))
            return FFLI_VERIFY_CHAR_INFO_REASON_CREATOR_NAME_INVALID;
    }

    if (pCharInfo->height > 128)
        return FFLI_VERIFY_CHAR_INFO_REASON_HEIGHT_INVALID;

    if (pCharInfo->build > 128)
        return FFLI_VERIFY_CHAR_INFO_REASON_BUILD_INVALID;

    if (pCharInfo->gender > 1)
        return FFLI_VERIFY_CHAR_INFO_REASON_GENDER_INVALID;

    if (!FFLiVerifyBirthday(pCharInfo->birthMonth, pCharInfo->birthDay))
        return FFLI_VERIFY_CHAR_INFO_REASON_BIRTHDAY_INVALID;

    if (pCharInfo->favoriteColor > 11)
        return FFLI_VERIFY_CHAR_INFO_REASON_FAVORITE_COLOR_INVALID;

    if (pCharInfo->regionMove > 3)
        return FFLI_VERIFY_CHAR_INFO_REASON_REGION_MOVE_INVALID;

    if (pCharInfo->fontRegion > 3)
        return FFLI_VERIFY_CHAR_INFO_REASON_FONT_REGION_INVALID;

    if (pCharInfo->pageIndex > 9)
        return FFLI_VERIFY_CHAR_INFO_REASON_PAGE_INDEX_INVALID;

    if (pCharInfo->slotIndex > 9)
        return FFLI_VERIFY_CHAR_INFO_REASON_SLOT_INDEX_INVALID;

    if (!(1 <= pCharInfo->birthPlatform && pCharInfo->birthPlatform <= 7))
        return FFLI_VERIFY_CHAR_INFO_REASON_BIRTH_PLATFORM_INVALID;

    if (FFLiIsValidMiiID(&pCharInfo->creatorID) && !FFLiIsNormalMiiID(&pCharInfo->creatorID) && !pCharInfo->localOnly)
        return FFLI_VERIFY_CHAR_INFO_REASON_CREATOR_ID_INVALID;

    return FFLI_VERIFY_CHAR_INFO_REASON_OK;
}

BOOL FFLiiVerifyCharInfo(const FFLiCharInfo* pCharInfo, BOOL verifyName)
{
    return FFLiVerifyCharInfoWithReason(pCharInfo, verifyName) == FFLI_VERIFY_CHAR_INFO_REASON_OK;
}

s32 FFLiiGetEyeRotateOffset(s32 type)
{
    static const u8 ROTATE[64] = {  // Don't ask me why this is 64 and not 60
        3, 4, 4, 4,
        3, 4, 4, 4,
        3, 4, 4, 4,
        4, 3, 3, 4,
        4, 4, 3, 3,
        4, 3, 4, 3,
        3, 4, 3, 4,
        4, 3, 4, 4,
        4, 3, 3, 3,
        4, 4, 3, 3,
        3, 4, 4, 3,
        3, 3, 3, 3,
        3, 3, 3, 3,
        4, 4, 4, 4,
        3, 4, 4, 3,
        4, 4, 0, 0
    };
    return 32 - ROTATE[type];
}

s32 FFLiiGetEyebrowRotateOffset(s32 type)
{
    static const u8 ROTATE[24] = {
        6, 6, 5, 7,
        6, 7, 6, 7,
        4, 7, 6, 8,
        5, 5, 6, 6,
        7, 7, 6, 6,
        5, 6, 7, 5
    };
    return 32 - ROTATE[type];
}

f32 FFLiiGetAdjustedMouthH(f32 height, s32 type)
{
    switch (type)
    {
    case 3:
    case 15:
    case 19:
    case 20:
    case 21:
    case 23:
    case 25:
        if (height < 12)
            height = 12;
    }
    return height;
}

f32 FFLiiGetAdjustedEyeH(f32 height, s32 type)
{
    switch (type)
    {
    case 14:
    case 26:
        if (height < 12)
            height = 12;
    }
    return height;
}

bool FFLiIsValidCharacterForName(u16 c)
{
    if (c == L'%' || c == L'\\')
        return false;

    return true;
}

bool FFLiCompareCharInfoWithAdditionalInfo(s32* pFlagOut, s32 flagIn, const FFLiCharInfo* pCharInfoA, const FFLiCharInfo* pCharInfoB, const FFLAdditionalInfo* pAdditionalInfoA, const FFLAdditionalInfo* pAdditionalInfoB)
{
    s32 flag = 0;
    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_PARTS)
    {
        if (pCharInfoA->parts.faceType != pCharInfoB->parts.faceType ||
            pCharInfoA->parts.facelineColor != pCharInfoB->parts.facelineColor ||
            pCharInfoA->parts.faceLine != pCharInfoB->parts.faceLine ||
            pCharInfoA->parts.faceMakeup != pCharInfoB->parts.faceMakeup ||
            pCharInfoA->parts.hairType != pCharInfoB->parts.hairType ||
            pCharInfoA->parts.hairColor != pCharInfoB->parts.hairColor ||
            pCharInfoA->parts.hairDir != pCharInfoB->parts.hairDir ||
            pCharInfoA->parts.eyeType != pCharInfoB->parts.eyeType ||
            pCharInfoA->parts.eyeColor != pCharInfoB->parts.eyeColor ||
            pCharInfoA->parts.eyeScale != pCharInfoB->parts.eyeScale ||
            pCharInfoA->parts.eyeScaleY != pCharInfoB->parts.eyeScaleY ||
            pCharInfoA->parts.eyeRotate != pCharInfoB->parts.eyeRotate ||
            pCharInfoA->parts.eyeSpacingX != pCharInfoB->parts.eyeSpacingX ||
            pCharInfoA->parts.eyePositionY != pCharInfoB->parts.eyePositionY ||
            pCharInfoA->parts.eyebrowType != pCharInfoB->parts.eyebrowType ||
            pCharInfoA->parts.eyebrowColor != pCharInfoB->parts.eyebrowColor ||
            pCharInfoA->parts.eyebrowScale != pCharInfoB->parts.eyebrowScale ||
            pCharInfoA->parts.eyebrowScaleY != pCharInfoB->parts.eyebrowScaleY ||
            pCharInfoA->parts.eyebrowRotate != pCharInfoB->parts.eyebrowRotate ||
            pCharInfoA->parts.eyebrowSpacingX != pCharInfoB->parts.eyebrowSpacingX ||
            pCharInfoA->parts.eyebrowPositionY != pCharInfoB->parts.eyebrowPositionY ||
            pCharInfoA->parts.noseType != pCharInfoB->parts.noseType ||
            pCharInfoA->parts.noseScale != pCharInfoB->parts.noseScale ||
            pCharInfoA->parts.nosePositionY != pCharInfoB->parts.nosePositionY ||
            pCharInfoA->parts.mouthType != pCharInfoB->parts.mouthType ||
            pCharInfoA->parts.mouthColor != pCharInfoB->parts.mouthColor ||
            pCharInfoA->parts.mouthScale != pCharInfoB->parts.mouthScale ||
            pCharInfoA->parts.mouthScaleY != pCharInfoB->parts.mouthScaleY ||
            pCharInfoA->parts.mouthPositionY != pCharInfoB->parts.mouthPositionY ||
            pCharInfoA->parts.mustacheType != pCharInfoB->parts.mustacheType ||
            pCharInfoA->parts.beardType != pCharInfoB->parts.beardType ||
            pCharInfoA->parts.beardColor != pCharInfoB->parts.beardColor ||
            pCharInfoA->parts.mustacheScale != pCharInfoB->parts.mustacheScale ||
            pCharInfoA->parts.mustachePositionY != pCharInfoB->parts.mustachePositionY ||
            pCharInfoA->parts.glassType != pCharInfoB->parts.glassType ||
            pCharInfoA->parts.glassColor != pCharInfoB->parts.glassColor ||
            pCharInfoA->parts.glassScale != pCharInfoB->parts.glassScale ||
            pCharInfoA->parts.glassPositionY != pCharInfoB->parts.glassPositionY ||
            pCharInfoA->parts.moleType != pCharInfoB->parts.moleType ||
            pCharInfoA->parts.moleScale != pCharInfoB->parts.moleScale ||
            pCharInfoA->parts.molePositionX != pCharInfoB->parts.molePositionX ||
            pCharInfoA->parts.molePositionY != pCharInfoB->parts.molePositionY)
        {
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_PARTS;
        }
    }

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_NAME)
        if (FFLiCompareString16(pAdditionalInfoA->name, pAdditionalInfoB->name, 10 + 1) != 0)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_NAME;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_CREATOR_NAME)
        if (FFLiCompareString16(pAdditionalInfoA->creatorName, pAdditionalInfoB->creatorName, 10 + 1) != 0)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_CREATOR_NAME;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_CREATOR_ID)
        if (!FFLiIsSameMiiID(&pAdditionalInfoA->creatorID, &pAdditionalInfoB->creatorID))
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_CREATOR_ID;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_GENDER)
        if (pAdditionalInfoA->gender != pAdditionalInfoB->gender)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_GENDER;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_BIRTH_MONTH)
        if (pAdditionalInfoA->birthMonth != pAdditionalInfoB->birthMonth)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_BIRTH_MONTH;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_BIRTH_DAY)
        if (pAdditionalInfoA->birthDay != pAdditionalInfoB->birthDay)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_BIRTH_DAY;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_FAVORITE_COLOR)
        if (pAdditionalInfoA->favoriteColor != pAdditionalInfoB->favoriteColor)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_FAVORITE_COLOR;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_HEIGHT)
        if (pAdditionalInfoA->height != pAdditionalInfoB->height)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_HEIGHT;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_BUILD)
        if (pAdditionalInfoA->build != pAdditionalInfoB->build)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_BUILD;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_FACELINE_COLOR)
        if (std::memcmp(&pAdditionalInfoA->facelineColor, &pAdditionalInfoB->facelineColor, sizeof(FFLColor)) != 0)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_FACELINE_COLOR;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_MII_VERSION)
        if (!CompareMiiVersion(pCharInfoA, pCharInfoB))
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_MII_VERSION;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_COPYABLE)
        if (!CompareCopyable(pCharInfoA, pCharInfoB))
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_COPYABLE;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_NG_WORD)
        if (pAdditionalInfoA->ngWord != pAdditionalInfoB->ngWord)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_NG_WORD;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_LOCAL_ONLY)
        if (!CompareLocalOnly(pCharInfoA, pCharInfoB))
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_LOCAL_ONLY;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_REGION_MOVE)
        if (!CompareRegionMove(pCharInfoA, pCharInfoB))
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_REGION_MOVE;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_FONT_REGION)
        if (pAdditionalInfoA->fontRegion != pAdditionalInfoB->fontRegion)
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_FONT_REGION;

    if (flagIn & FFLI_COMPARE_CHAR_INFO_FLAG_BIRTH_PLATFORM)
        if (!CompareBirthPlatform(pCharInfoA, pCharInfoB))
            flag |= FFLI_COMPARE_CHAR_INFO_FLAG_BIRTH_PLATFORM;

    if (pFlagOut != NULL)
        *pFlagOut = flag;

    return flag != 0;
}

namespace {

bool VerifyName(const u16* s, s32 size)
{
    for (s32 i = 0; i < size; i++)
    {
        u16 c = s[i];
        if (c == L'\0')
            break;

        if (!FFLiIsValidCharacterForName(c))
            return false;
    }

    return true;
}

bool CompareMiiVersion(const FFLiCharInfo* a, const FFLiCharInfo* b)
{
    return a->miiVersion == b->miiVersion;
}

bool CompareCopyable(const FFLiCharInfo* a, const FFLiCharInfo* b)
{
    return a->copyable == b->copyable;
}

bool CompareLocalOnly(const FFLiCharInfo* a, const FFLiCharInfo* b)
{
    return a->localOnly == b->localOnly;
}

bool CompareRegionMove(const FFLiCharInfo* a, const FFLiCharInfo* b)
{
    return a->regionMove == b->regionMove;
}

bool CompareBirthPlatform(const FFLiCharInfo* a, const FFLiCharInfo* b)
{
    return a->birthPlatform == b->birthPlatform;
}

}
