#ifndef FFLI_MIIDATA_CORE_H_
#define FFLI_MIIDATA_CORE_H_

#include <nn/ffl/types.h>

class FFLiMiiDataCore
{
private:
    u32 _0;                 // Bitfield
    
    u8  _4[8];
    
    u8  _c[10];             // Wstring
    u8  _padding1[2];       // (For null-terminator?)
    
    u16 _18;                // Bitfield
    
    u16 _1a[10];            // Wstring
    u8  _padding2[2];       // (For null-terminator?)

    u16 _30[12];            // Bitfield array
};
NN_STATIC_ASSERT_IS_POD(FFLiMiiDataCore);
NN_STATIC_ASSERT(sizeof(FFLiMiiDataCore) == 0x48);

class FFLiMiiDataOfficial : public FFLiMiiDataCore
{
private:
    u16 creatorName[10];    // Wstring
};
NN_STATIC_ASSERT_IS_POD(FFLiMiiDataOfficial);
NN_STATIC_ASSERT(sizeof(FFLiMiiDataOfficial) == 0x5C);

#endif // FFLI_MIIDATA_CORE_H_