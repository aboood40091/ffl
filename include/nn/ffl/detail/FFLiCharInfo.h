#ifndef FFLI_CHARINFO_H_
#define FFLI_CHARINFO_H_

#include <nn/ffl/types.h>

#ifdef __cplusplus

#include <nn/ffl/FFLBirthPlatform.h>
#include <nn/ffl/FFLCreateID.h>
#include <nn/ffl/FFLFontRegion.h>
#include <nn/ffl/FFLGender.h>

#include <nn/ffl/FFLiAuthorID.h>

struct FFLiCharInfo
{
    u32                 miiVersion;
    struct
    {
        s32             faceType;
        s32             facelineColor;
        s32             faceLine;
        s32             faceMakeup;
        s32             hairType;
        s32             hairColor;
        s32             hairDir;
        s32             eyeType;
        s32             eyeColor;
        s32             eyeScale;
        s32             eyeScaleY;
        s32             eyeRotate;
        s32             eyeSpacingX;
        s32             eyePositionY;
        s32             eyebrowType;
        s32             eyebrowColor;
        s32             eyebrowScale;
        s32             eyebrowScaleY;
        s32             eyebrowRotate;
        s32             eyebrowSpacingX;
        s32             eyebrowPositionY;
        s32             noseType;
        s32             noseScale;
        s32             nosePositionY;
        s32             mouthType;
        s32             mouthColor;
        s32             mouthScale;
        s32             mouthScaleY;
        s32             mouthPositionY;
        s32             mustacheType;
        s32             beardType;
        s32             beardColor;
        s32             mustacheScale;
        s32             mustachePositionY;
        s32             glassType;
        s32             glassColor;
        s32             glassScale;
        s32             glassPositionY;
        s32             moleType;
        s32             moleScale;
        s32             molePositionX;
        s32             molePositionY;
    } parts;
    u32                 height;
    u32                 build;
    u16                 name[10 + 1];
    u16                 creatorName[10 + 1];
    FFLGender           gender;
    u32                 birthMonth;
    u32                 birthDay;
    u32                 favoriteColor;
    bool                favoriteMii;
    bool                copyable;
    u8                  ngWord;
    bool                localOnly;
    u32                 regionMove;
    FFLFontRegion       fontRegion;
    u32                 pageIndex;
    u32                 slotIndex;
    FFLBirthPlatform    birthPlatform;
    FFLCreateID         createID;
    u16                 _112;   // Set to zero
    u32                 _114;   // Set to FFLiMiiDataCore._0_24_27
    FFLiAuthorID        authorID;
};
NN_STATIC_ASSERT_IS_POD(FFLiCharInfo);
NN_STATIC_ASSERT(sizeof(FFLiCharInfo) == 0x120);

BOOL FFLiiVerifyCharInfo(const FFLiCharInfo* pCharInfo, BOOL verifyName);

extern "C" {

#else

typedef struct FFLiCharInfo FFLiCharInfo;

#endif

BOOL FFLiVerifyCharInfo(const FFLiCharInfo* pCharInfo, BOOL verifyName);

#ifdef __cplusplus

}

s32 FFLiiGetEyeRotateOffset(s32 type);
s32 FFLiiGetEyebrowRotateOffset(s32 type);
f32 FFLiiGetAdjustedMouthH(f32 height, s32 type);
f32 FFLiiGetAdjustedEyeH(f32 height, s32 type);

struct FFLAdditionalInfo;

enum FFLiCompareCharInfoFlagsIn
{
    FFLI_COMPARE_CHAR_INFO_FLAGS_IN_COMPARE_PARTS = 1 << 0
};

bool FFLiCompareCharInfoWithAdditionalInfo(s32* pFlagsOut, s32 flagsIn, const FFLiCharInfo* pCharInfoA, const FFLiCharInfo* pCharInfoB, const FFLAdditionalInfo* pAdditionalInfoA, const FFLAdditionalInfo* pAdditionalInfoB);

#endif

#endif // FFLI_CHARINFO_H_
