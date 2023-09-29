#ifndef FFLI_MIIDATA_CORE_H_
#define FFLI_MIIDATA_CORE_H_

#include <nn/ffl/FFLCreateID.h>
#include <nn/ffl/FFLStandard.h>

#include <nn/ffl/FFLiAuthorID.h>

class FFLiMiiDataCore
{
private:
    union
    {
        struct
        {
            u32  m_MiiVersion    : 8;
            u32  m_Copyable      : 1;    // bool
            u32  m_NgWord        : 1;    // bool
            u32  m_RegionMove    : 2;
            u32  m_FontRegion    : 2;
            u32                  : 2;    // Unused
            u32  m_PageIndex     : 4;
            u32  m_SlotIndex     : 4;
            u32  _0_24_27        : 4;
            u32  m_BirthPlatform : 3;
        };
        
        u32 _0;
    };

    union
    {
        FFLiAuthorID    m_AuthorID;
        
        u8  _4[8];
    };

    union
    {
        FFLCreateID m_CreateID;
        
        u8  _C[0xA];
    };
    u8  _padding1[2];

    union
    {
        struct
        {
            u16 m_Gender        : 1;
            u16 m_BirthMonth    : 4;
            u16 m_BirthDay      : 5;
            u16 m_FavoriteColor : 4;
            u16 m_FavoriteMii   : 1;    // bool
        };

        u16 _18;
    };
    
    u16 m_Name[10]; // Wstring
    
    u8  m_Height;
    u8  m_Build;

    union
    {
        struct
        {
            u16 m_LocalOnly     : 1;    // bool
            u16 m_FaceType      : 4;
            u16 m_FacelineColor : 3;
            u16 m_FaceLine      : 4;
            u16 m_FaceMakeup    : 4;
        };
        
        u16 _30;
    };
    union
    {
        struct
        {
            u16 m_HairType  : 8;
            u16 m_HairColor : 3;
            u16 m_HairDir   : 1;
        };
        
        u16 _32;
    };
    union
    {
        struct
        {
            u16 m_EyeType   : 6;
            u16 m_EyeColor  : 3;
            u16 m_EyeScaleX : 4;
            u16 m_EyeScaleY : 3;
        };
        
        u16 _34;
    };
    union
    {
        struct
        {
            u16 m_EyeRotateOffset   : 5;
            u16 m_EyeSpacingX       : 4;
            u16 m_EyePositionY      : 5;
        };
        
        u16 _36;
    };
    union
    {
        struct
        {
            u16 m_EyebrowType   : 5;
            u16 m_EyebrowColor  : 3;
            u16 m_EyebrowScaleX : 4;
            u16 m_EyebrowScaleY : 3;
        };
        
        u16 _38;
    };
    union
    {
        struct
        {
            u16 m_EyebrowRotateOffset   : 5;
            u16 m_EyebrowSpacingX       : 4;
            u16 m_EyebrowPositionY      : 5;
        };
        
        u16 _3A;
    };
    union
    {
        struct
        {
            u16 m_NoseType      : 5;
            u16 m_NoseScale     : 4;
            u16 m_NosePositionY : 5;
        };
        
        u16 _3C;
    };
    union
    {
        struct
        {
            u16 m_MouthType     : 6;
            u16 m_MouthColor    : 3;
            u16 m_MouthScaleX   : 4;
            u16 m_MouthScaleY   : 3;
        };
        
        u16 _3E;
    };
    union
    {
        struct
        {
            u16 m_MouthPositionY    : 5;
            u16 m_MustacheType      : 3;
        };
        
        u16 _40;
    };
    union
    {
        struct
        {
            u16 m_BeardType         : 3;
            u16 m_BeardColor        : 3;
            u16 m_MustacheScale     : 4;
            u16 m_MustachePositionY : 5;
        };
        
        u16 _42;
    };
    union
    {
        struct
        {
            u16 m_GlassType         : 4;
            u16 m_GlassColor        : 3;
            u16 m_GlassScale        : 4;
            u16 m_GlassPositionY    : 5;
        };
        
        u16 _44;
    };
    union
    {
        struct
        {
            u16 m_MoleType      : 1;
            u16 m_MoleScale     : 4;
            u16 m_MolePositionX : 5;
            u16 m_MolePositionY : 5;
        };
        
        u16 _46;
    };
};
NN_STATIC_ASSERT_IS_POD(FFLiMiiDataCore);
NN_STATIC_ASSERT(sizeof(FFLiMiiDataCore) == 0x48);

class FFLiMiiDataOfficial : public FFLiMiiDataCore
{
private:
    u16 m_CreatorName[10];  // Wstring
};
NN_STATIC_ASSERT_IS_POD(FFLiMiiDataOfficial);
NN_STATIC_ASSERT(sizeof(FFLiMiiDataOfficial) == 0x5C);

#define FFL_STOREDATA_SIZE  (0x60)

class FFLiStoreData : public FFLiMiiDataOfficial
{
private:
    u16 _5c;
    u16 m_Crc;
};
NN_STATIC_ASSERT_IS_POD(FFLiStoreData);
NN_STATIC_ASSERT(sizeof(FFLiStoreData) == FFL_STOREDATA_SIZE);

struct FFLiStoreDataCFL : FFLStoreData
{
};
NN_STATIC_ASSERT_IS_POD(FFLiStoreDataCFL);
NN_STATIC_ASSERT(sizeof(FFLiStoreDataCFL) == FFL_STOREDATA_SIZE);

class FFLiMiiDataHidden : public FFLiMiiDataCore
{
private:
    u32 _48[12 / sizeof(u32)];  // Actual type unknown
};
NN_STATIC_ASSERT_IS_POD(FFLiMiiDataHidden);
NN_STATIC_ASSERT(sizeof(FFLiMiiDataHidden) == 0x54);

#endif // FFLI_MIIDATA_CORE_H_