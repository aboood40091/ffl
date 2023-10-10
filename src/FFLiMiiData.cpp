#include <nn/ffl/FFLiCreateID.h>
#include <nn/ffl/FFLiMiiData.h>
#include <nn/ffl/FFLiMiiDataCore.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiCharInfo.h>
#include <nn/ffl/detail/FFLiCrc.h>

#include <cstring>

static inline bool IsInvalidCharCode(u16 c)
{
    if (c == L'%'                       ||
        c == L'@'                       ||
        c == L'\\'                      ||
        c == 0xFF20                     ||  // 0xFF20 == L'＠', but I can't get it working with GHS
        (0xE000 <= c && c <= 0xF8FF))       // Private-use characters
    {
        return true;
    }
    return false;
}

namespace {

const s32* RflToFaceLineAndMake(s32 type);

bool ReplaceNameFromRFL(u16* s, s32 size, s32 maxNumberCharCount);

}

bool FFLiMiiDataCoreRFL2MiiDataCore(FFLiMiiDataCore* pMiiDataCore, const FFLiMiiDataCoreRFL& miiDataCoreRFL, bool replaceName)
{
    FFLiCharInfo charInfo;
    bool ret = FFLiMiiDataCoreRFL2CharInfo(&charInfo, miiDataCoreRFL, NULL, replaceName);
    FFLiCharInfo2MiiDataCore(pMiiDataCore, charInfo, false);
    return ret;
}

bool FFLiMiiDataCoreRFL2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataCoreRFL& miiDataCoreRFL, const u16* pCreatorName, bool replaceName)
{
    pCharInfo->miiVersion = 3;

    pCharInfo->parts.faceType = miiDataCoreRFL.FaceType();
    pCharInfo->parts.facelineColor = miiDataCoreRFL.FacelineColor();
    pCharInfo->parts.faceLine = RflToFaceLineAndMake(miiDataCoreRFL.FaceLineAndMake())[0];
    pCharInfo->parts.faceMakeup = RflToFaceLineAndMake(miiDataCoreRFL.FaceLineAndMake())[1];

    pCharInfo->parts.hairType = miiDataCoreRFL.HairType();
    pCharInfo->parts.hairColor = miiDataCoreRFL.HairColor();
    pCharInfo->parts.hairDir = miiDataCoreRFL.HairDir();

    pCharInfo->parts.eyeType = miiDataCoreRFL.EyeType();
    pCharInfo->parts.eyeColor = miiDataCoreRFL.EyeColor();
    pCharInfo->parts.eyeScale = miiDataCoreRFL.EyeScale();
    pCharInfo->parts.eyeScaleY = 3;
    pCharInfo->parts.eyeRotate = miiDataCoreRFL.EyeRotate();
    pCharInfo->parts.eyeSpacingX = miiDataCoreRFL.EyeSpacing();
    pCharInfo->parts.eyePositionY = miiDataCoreRFL.EyePosition();

    pCharInfo->parts.eyebrowType = miiDataCoreRFL.EyebrowType();
    pCharInfo->parts.eyebrowColor = miiDataCoreRFL.EyebrowColor();
    pCharInfo->parts.eyebrowScale = miiDataCoreRFL.EyebrowScale();
    pCharInfo->parts.eyebrowScaleY = 3;
    pCharInfo->parts.eyebrowRotate = miiDataCoreRFL.EyebrowRotate();
    pCharInfo->parts.eyebrowSpacingX = miiDataCoreRFL.EyebrowSpacing();
    pCharInfo->parts.eyebrowPositionY = miiDataCoreRFL.EyebrowPosition();

    pCharInfo->parts.noseType = miiDataCoreRFL.NoseType();
    pCharInfo->parts.noseScale = miiDataCoreRFL.NoseScale();
    pCharInfo->parts.nosePositionY = miiDataCoreRFL.NosePosition();

    pCharInfo->parts.mouthType = miiDataCoreRFL.MouthType();
    pCharInfo->parts.mouthColor = miiDataCoreRFL.MouthColor();
    pCharInfo->parts.mouthScale = miiDataCoreRFL.MouthScale();
    pCharInfo->parts.mouthScaleY = 3;
    pCharInfo->parts.mouthPositionY = miiDataCoreRFL.MouthPosition();

    pCharInfo->parts.mustacheType = miiDataCoreRFL.MustacheType();
    pCharInfo->parts.beardType = miiDataCoreRFL.BeardType();
    pCharInfo->parts.beardColor = miiDataCoreRFL.BeardColor();
    pCharInfo->parts.mustacheScale = miiDataCoreRFL.MustacheScale();
    pCharInfo->parts.mustachePositionY = miiDataCoreRFL.MustachePosition();

    pCharInfo->parts.glassType = miiDataCoreRFL.GlassType();
    pCharInfo->parts.glassColor = miiDataCoreRFL.GlassColor();
    pCharInfo->parts.glassScale = miiDataCoreRFL.GlassScale();
    pCharInfo->parts.glassPositionY = miiDataCoreRFL.GlassPosition();

    pCharInfo->parts.moleType = miiDataCoreRFL.MoleType();
    pCharInfo->parts.moleScale = miiDataCoreRFL.MoleScale();
    pCharInfo->parts.molePositionX = miiDataCoreRFL.MolePositionX();
    pCharInfo->parts.molePositionY = miiDataCoreRFL.MolePositionY();

    pCharInfo->height = miiDataCoreRFL.Height();
    pCharInfo->build = miiDataCoreRFL.Build();

    pCharInfo->gender = FFLGender(miiDataCoreRFL.Gender());

    pCharInfo->favoriteColor = miiDataCoreRFL.FavoriteColor();

    pCharInfo->favoriteMii = miiDataCoreRFL.FavoriteMii();
    pCharInfo->copyable = false;
    pCharInfo->ngWord = false;
    pCharInfo->localOnly = miiDataCoreRFL.LocalOnly();

    pCharInfo->regionMove = 0;
    pCharInfo->fontRegion = FFL_FONT_REGION_0;

    pCharInfo->pageIndex = 0;
    pCharInfo->slotIndex = 0;

    pCharInfo->_114 = 0;

    std::memset(&pCharInfo->authorID, 0, sizeof(FFLiAuthorID));

    std::memcpy(pCharInfo->name, miiDataCoreRFL.Name(), sizeof(u16) * 10);
    pCharInfo->name[10] = L'\0';
    bool nameReplaced = replaceName ? ReplaceNameFromRFL(pCharInfo->name, 10, 5) : false;

    std::memset(&pCharInfo->createID, 0, sizeof(FFLCreateID));                          // Create ID is cleared here...

    pCharInfo->birthMonth = miiDataCoreRFL.BirthMonth();
    pCharInfo->birthDay = miiDataCoreRFL.BirthDay();

    if (pCreatorName != NULL)
        std::memcpy(pCharInfo->creatorName, pCreatorName, sizeof(u16) * (10 + 1));
    else
        std::memset(pCharInfo->creatorName, 0, sizeof(u16) * (10 + 1));
    pCharInfo->creatorName[10] = L'\0';
    bool creatorNameReplaced = replaceName ? ReplaceNameFromRFL(pCharInfo->creatorName, 10, 0) : false;

    bool isNTR = FFLiIsNTRMiiID(&pCharInfo->createID);
    pCharInfo->birthPlatform = isNTR ? FFL_BIRTH_PLATFORM_NTR : FFL_BIRTH_PLATFORM_WII; //  ... yet they still check it LOL

    pCharInfo->_112 = 0;

    return nameReplaced || creatorNameReplaced;
}

void FFLiClearCreatorNameFromOfficial(FFLiMiiDataOfficial* pMiiDataOfficial)
{
    std::memset(pMiiDataOfficial->CreatorName(), 0, sizeof(u16) * 10);
}

void FFLiMiiDataCore2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataCore& miiDataCore, const u16* pCreatorName, bool resetBirthday)
{
    pCharInfo->miiVersion = miiDataCore.MiiVersion();

    pCharInfo->parts.faceType = miiDataCore.FaceType();
    pCharInfo->parts.facelineColor = miiDataCore.FacelineColor();
    pCharInfo->parts.faceLine = miiDataCore.FaceLine();
    pCharInfo->parts.faceMakeup = miiDataCore.FaceMake();

    pCharInfo->parts.hairType = miiDataCore.HairType();
    pCharInfo->parts.hairColor = miiDataCore.HairColor();
    pCharInfo->parts.hairDir = miiDataCore.HairDir();

    pCharInfo->parts.eyeType = miiDataCore.EyeType();
    pCharInfo->parts.eyeColor = miiDataCore.EyeColor();
    pCharInfo->parts.eyeScale = miiDataCore.EyeScale();
    pCharInfo->parts.eyeScaleY = miiDataCore.EyeScaleY();
    pCharInfo->parts.eyeRotate = miiDataCore.EyeRotate();
    pCharInfo->parts.eyeSpacingX = miiDataCore.EyeSpacing();
    pCharInfo->parts.eyePositionY = miiDataCore.EyePosition();

    pCharInfo->parts.eyebrowType = miiDataCore.EyebrowType();
    pCharInfo->parts.eyebrowColor = miiDataCore.EyebrowColor();
    pCharInfo->parts.eyebrowScale = miiDataCore.EyebrowScale();
    pCharInfo->parts.eyebrowScaleY = miiDataCore.EyebrowScaleY();
    pCharInfo->parts.eyebrowRotate = miiDataCore.EyebrowRotate();
    pCharInfo->parts.eyebrowSpacingX = miiDataCore.EyebrowSpacing();
    pCharInfo->parts.eyebrowPositionY = miiDataCore.EyebrowPosition();

    pCharInfo->parts.noseType = miiDataCore.NoseType();
    pCharInfo->parts.noseScale = miiDataCore.NoseScale();
    pCharInfo->parts.nosePositionY = miiDataCore.NosePosition();

    pCharInfo->parts.mouthType = miiDataCore.MouthType();
    pCharInfo->parts.mouthColor = miiDataCore.MouthColor();
    pCharInfo->parts.mouthScale = miiDataCore.MouthScale();
    pCharInfo->parts.mouthScaleY = miiDataCore.MouthScaleY();
    pCharInfo->parts.mouthPositionY = miiDataCore.MouthPosition();

    pCharInfo->parts.mustacheType = miiDataCore.MustacheType();
    pCharInfo->parts.beardType = miiDataCore.BeardType();
    pCharInfo->parts.beardColor = miiDataCore.BeardColor();
    pCharInfo->parts.mustacheScale = miiDataCore.MustacheScale();
    pCharInfo->parts.mustachePositionY = miiDataCore.MustachePosition();

    pCharInfo->parts.glassType = miiDataCore.GlassType();
    pCharInfo->parts.glassColor = miiDataCore.GlassColor();
    pCharInfo->parts.glassScale = miiDataCore.GlassScale();
    pCharInfo->parts.glassPositionY = miiDataCore.GlassPosition();

    pCharInfo->parts.moleType = miiDataCore.MoleType();
    pCharInfo->parts.moleScale = miiDataCore.MoleScale();
    pCharInfo->parts.molePositionX = miiDataCore.MolePositionX();
    pCharInfo->parts.molePositionY = miiDataCore.MolePositionY();

    pCharInfo->height = miiDataCore.Height();
    pCharInfo->build = miiDataCore.Build();

    pCharInfo->gender = FFLGender(miiDataCore.Gender());

    pCharInfo->favoriteColor = miiDataCore.FavoriteColor();

    pCharInfo->favoriteMii = miiDataCore.FavoriteMii();
    pCharInfo->copyable = miiDataCore.Copyable();
    pCharInfo->ngWord = miiDataCore.NgWord();
    pCharInfo->localOnly = miiDataCore.LocalOnly();

    pCharInfo->regionMove = miiDataCore.RegionMove();
    pCharInfo->fontRegion = FFLFontRegion(miiDataCore.FontRegion());

    pCharInfo->pageIndex = miiDataCore.PageIndex();
    pCharInfo->slotIndex = miiDataCore.SlotIndex();

    pCharInfo->birthPlatform = FFLBirthPlatform(miiDataCore.BirthPlatform());

    pCharInfo->_114 = miiDataCore.FlagBit24To27();

    std::memcpy(&pCharInfo->authorID, &miiDataCore.AuthorID(), sizeof(FFLiAuthorID));

    std::memcpy(pCharInfo->name, miiDataCore.Name(), sizeof(u16) * (10 + 1));
    pCharInfo->name[10] = L'\0';

    std::memcpy(&pCharInfo->createID, &miiDataCore.CreateID(), sizeof(FFLCreateID));

    pCharInfo->birthMonth = resetBirthday ? 0 : miiDataCore.BirthMonth();
    pCharInfo->birthDay = resetBirthday ? 0 : miiDataCore.BirthDay();

    if (pCreatorName != NULL)
        std::memcpy(pCharInfo->creatorName, pCreatorName, sizeof(u16) * (10 + 1));
    else
        std::memset(pCharInfo->creatorName, 0, sizeof(u16) * (10 + 1));
    pCharInfo->creatorName[10] = L'\0';

    pCharInfo->_112 = 0;
}

void FFLiCharInfo2MiiDataCore(FFLiMiiDataCore* pMiiDataCore, const FFLiCharInfo& charInfo, bool resetBirthday)
{
    pMiiDataCore->SetMiiVersion(charInfo.miiVersion);
    pMiiDataCore->SetCopyable(charInfo.copyable);
    pMiiDataCore->SetNgWord(charInfo.ngWord);
    pMiiDataCore->SetRegionMove(charInfo.regionMove);
    pMiiDataCore->SetLocalOnly(charInfo.localOnly);
    pMiiDataCore->SetFontRegion(charInfo.fontRegion);
    pMiiDataCore->SetPageIndex(charInfo.pageIndex);
    pMiiDataCore->SetSlotIndex(charInfo.slotIndex);
    pMiiDataCore->SetFlagBit24To27(charInfo._114);
    pMiiDataCore->SetBirthPlatform(charInfo.birthPlatform);
    std::memcpy(&pMiiDataCore->AuthorID(), &charInfo.authorID, sizeof(FFLiAuthorID));
    std::memcpy(&pMiiDataCore->CreateID(), &charInfo.createID, sizeof(FFLCreateID));
    std::memset(pMiiDataCore->Padding1(), 0, pMiiDataCore->Padding1Size());
    pMiiDataCore->SetGender(charInfo.gender);
    pMiiDataCore->SetBirthMonth(resetBirthday ? 0 : charInfo.birthMonth);
    pMiiDataCore->SetBirthDay(resetBirthday ? 0 : charInfo.birthDay);
    pMiiDataCore->SetFavoriteColor(charInfo.favoriteColor);
    pMiiDataCore->SetFavoriteMii(charInfo.favoriteMii);
    std::memcpy(pMiiDataCore->Name(), charInfo.name, sizeof(u16) * 10);
    pMiiDataCore->SetHeight(charInfo.height);
    pMiiDataCore->SetBuild(charInfo.build);
    pMiiDataCore->SetFaceType(charInfo.parts.faceType);
    pMiiDataCore->SetFacelineColor(charInfo.parts.facelineColor);
    pMiiDataCore->SetFaceLine(charInfo.parts.faceLine);
    pMiiDataCore->SetFaceMake(charInfo.parts.faceMakeup);
    pMiiDataCore->SetHairType(charInfo.parts.hairType);
    pMiiDataCore->SetHairColor(charInfo.parts.hairColor);
    pMiiDataCore->SetHairDir(charInfo.parts.hairDir);
    pMiiDataCore->SetEyeType(charInfo.parts.eyeType);
    pMiiDataCore->SetEyeColor(charInfo.parts.eyeColor);
    pMiiDataCore->SetEyeScale(charInfo.parts.eyeScale);
    pMiiDataCore->SetEyeScaleY(charInfo.parts.eyeScaleY);
    pMiiDataCore->SetEyeRotate(charInfo.parts.eyeRotate);
    pMiiDataCore->SetEyeSpacing(charInfo.parts.eyeSpacingX);
    pMiiDataCore->SetEyePosition(charInfo.parts.eyePositionY);
    pMiiDataCore->SetEyebrowType(charInfo.parts.eyebrowType);
    pMiiDataCore->SetEyebrowColor(charInfo.parts.eyebrowColor);
    pMiiDataCore->SetEyebrowScale(charInfo.parts.eyebrowScale);
    pMiiDataCore->SetEyebrowScaleY(charInfo.parts.eyebrowScaleY);
    pMiiDataCore->SetEyebrowRotate(charInfo.parts.eyebrowRotate);
    pMiiDataCore->SetEyebrowSpacing(charInfo.parts.eyebrowSpacingX);
    pMiiDataCore->SetEyebrowPosition(charInfo.parts.eyebrowPositionY);
    pMiiDataCore->SetNoseType(charInfo.parts.noseType);
    pMiiDataCore->SetNoseScale(charInfo.parts.noseScale);
    pMiiDataCore->SetNosePosition(charInfo.parts.nosePositionY);
    pMiiDataCore->SetMouthType(charInfo.parts.mouthType);
    pMiiDataCore->SetMouthColor(charInfo.parts.mouthColor);
    pMiiDataCore->SetMouthScale(charInfo.parts.mouthScale);
    pMiiDataCore->SetMouthScaleY(charInfo.parts.mouthScaleY);
    pMiiDataCore->SetMouthPosition(charInfo.parts.mouthPositionY);
    pMiiDataCore->SetMustacheType(charInfo.parts.mustacheType);
    pMiiDataCore->SetBeardType(charInfo.parts.beardType);
    pMiiDataCore->SetBeardColor(charInfo.parts.beardColor);
    pMiiDataCore->SetMustacheScale(charInfo.parts.mustacheScale);
    pMiiDataCore->SetMustachePosition(charInfo.parts.mustachePositionY);
    pMiiDataCore->SetGlassType(charInfo.parts.glassType);
    pMiiDataCore->SetGlassColor(charInfo.parts.glassColor);
    pMiiDataCore->SetGlassScale(charInfo.parts.glassScale);
    pMiiDataCore->SetGlassPosition(charInfo.parts.glassPositionY);
    pMiiDataCore->SetMoleType(charInfo.parts.moleType);
    pMiiDataCore->SetMoleScale(charInfo.parts.moleScale);
    pMiiDataCore->SetMolePositionX(charInfo.parts.molePositionX);
    pMiiDataCore->SetMolePositionY(charInfo.parts.molePositionY);
    pMiiDataCore->SetPlaceHolder(0);
}

void FFLiMiiDataOfficial2CharInfo(FFLiCharInfo* pCharInfo, const FFLiMiiDataOfficial& miiDataOfficial)
{
    FFLiMiiDataCore2CharInfo(pCharInfo, miiDataOfficial, miiDataOfficial.CreatorName(), false);
}

void FFLiCharInfo2MiiDataOfficial(FFLiMiiDataOfficial* pMiiDataOfficial, const FFLiCharInfo& charInfo, bool resetBirthday)
{
    FFLiCharInfo2MiiDataCore(pMiiDataOfficial, charInfo, resetBirthday);
    std::memcpy(pMiiDataOfficial->CreatorName(), charInfo.creatorName, sizeof(u16) * 10);
}

FFLResult FFLiStoreDataCFLToCharInfo(FFLiCharInfo* pCharInfo, const FFLiStoreDataCFL& storeDataCFL)
{
    if (!FFLiIsValidCRC16(&storeDataCFL, sizeof(FFLiStoreDataCFL)))
        return FFL_RESULT_UNKNOWN_3;

    FFLiStoreData storeData;
    std::memcpy(&storeData, &storeDataCFL, FFL_STOREDATA_SIZE);
    storeData.SwapEndian();
    FFLiMiiDataOfficial2CharInfo(pCharInfo, storeData);
    return FFL_RESULT_OK;
}

FFLResult FFLiCharInfoToStoreDataCFL(FFLiStoreDataCFL* pStoreDataCFL, const FFLiCharInfo* pCharInfo)
{
    FFLiMiiDataOfficial miiDataOfficial;
    FFLiCharInfo2MiiDataOfficial(&miiDataOfficial, *pCharInfo, false);
    FFLiMiiDataOfficialToStoreDataCFL(*pStoreDataCFL, miiDataOfficial);
    return FFL_RESULT_OK;
}

void FFLiMiiDataOfficialToStoreDataCFL(FFLiStoreDataCFL& storeDataCFL, const FFLiMiiDataOfficial& miiDataOfficial)
{
    FFLiStoreData storeData;
    std::memset(&storeData, 0, FFL_STOREDATA_SIZE);
    static_cast<FFLiMiiDataOfficial&>(storeData) = miiDataOfficial;
    storeData.SetCRC();
    storeData.SwapEndian();
    std::memcpy(&storeDataCFL, &storeData, FFL_STOREDATA_SIZE);
}

namespace {

static const s32 RFL_FACE_LINE_AND_MAKE[12][2] = {
    {  0,  0 },
    {  0,  1 },
    {  0,  6 },
    {  0,  9 },
    {  5,  0 },
    {  2,  0 },
    {  3,  0 },
    {  7,  0 },
    {  8,  0 },
    {  0, 10 },
    {  9,  0 },
    { 11,  0 }
};

const s32* RflToFaceLineAndMake(s32 type)
{
    return RFL_FACE_LINE_AND_MAKE[type];
}

bool VerifyNameFromRFL(u16* s, s32 size, s32 maxNumberCharCount)
{
    s32 numberCharCount = 0;

    for (s32 i = 0; i < size; i++)
    {
        u16 c = s[i];
        if (c == L'\0')
            break;

        if (IsInvalidCharCode(c))
            return false;

        if (FFLiIsNumberCharCode(c) && ++numberCharCount > maxNumberCharCount)
            return false;
    }

    return true;
}

bool ReplaceNameFromRFL(u16* s, s32 size, s32 maxNumberCharCount)
{
    if (!VerifyNameFromRFL(s, size, maxNumberCharCount))
    {
        FFLiCopyWcharT2U16(s, size, L"???");
        return true;
    }
    return false;
}

}
