#ifndef FFLI_CHARINFO_H_
#define FFLI_CHARINFO_H_

#include <nn/ffl/FFLCreateID.h>
#include <nn/ffl/FFLFontRegion.h>
#include <nn/ffl/FFLGender.h>

#include <nn/ffl/FFLiAuthorID.h>

struct FFLiCharInfo
{
    u32             miiVersion;
    struct
    {
        u32         faceType;
        u32         facelineColor;
        u32         faceLine;
        u32         faceMakeup;
        u32         hairType;
        u32         hairColor;
        u32         hairDir;
        u32         eyeType;
        u32         eyeColor;
        u32         eyeScaleX;
        u32         eyeScaleY;
        u32         eyeRotateOffset;
        u32         eyeSpacingX;
        u32         eyePositionY;
        u32         eyebrowType;
        u32         eyebrowColor;
        u32         eyebrowScaleX;
        u32         eyebrowScaleY;
        u32         eyebrowRotateOffset;
        u32         eyebrowSpacingX;
        u32         eyebrowPositionY;
        u32         noseType;
        s32         noseScale;
        s32         nosePositionY;
        u32         mouthType;
        u32         mouthColor;
        u32         mouthScaleX;
        u32         mouthScaleY;
        u32         mouthPositionY;
        u32         mustacheType;
        u32         beardType;
        s32         beardColor;
        u32         mustacheScale;
        u32         mustachePositionY;
        u32         glassType;
        u32         glassColor;
        s32         glassScale;
        s32         glassPositionY;
        u32         moleType;
        u32         moleScale;
        u32         molePositionX;
        u32         molePositionY;
    } parts;
    u32             height;
    u32             build;
    u16             name[10 + 1];
    u16             creatorName[10 + 1];
    FFLGender       gender;
    u32             birthMonth;
    u32             birthDay;
    u32             favoriteColor;
    bool            favoriteMii;
    bool            copyable;
    u8              ngWord;
    bool            localOnly;
    u32             regionMove;
    FFLFontRegion   fontRegion;
    u32             pageIndex;
    u32             slotIndex;
    u32             birthPlatform;
    FFLCreateID     createID;
    u16             _112;   // Set to zero
    u32             _114;   // Set to FFLiMiiDataCore._0_24_27
    FFLiAuthorID    authorID;
};
NN_STATIC_ASSERT(sizeof(FFLiCharInfo) == 0x120);

#endif // FFLI_CHARINFO_H_
