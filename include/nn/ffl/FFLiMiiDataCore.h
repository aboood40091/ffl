#ifndef FFLI_MIIDATA_CORE_H_
#define FFLI_MIIDATA_CORE_H_

#include <nn/ffl/FFLCreateID.h>

#include <nn/ffl/FFLiAuthorID.h>

class FFLiMiiDataCore
{
private:
    u32             _0;             // 0x00: Bitfield

    FFLiAuthorID    m_AuthorID;     // 0x04

    FFLCreateID     m_CreateID;     // 0x0C
    u8              _padding1[2];

    u16             _18;            // 0x18: Bitfield
    u16             m_Name[10];     //       Wstring
    u8              _padding2[2];

    u16             _30[12];        // 0x30: Bitfield array
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

#endif // FFLI_MIIDATA_CORE_H_