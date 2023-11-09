#pragma once

#include <nn/ffl/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FFL_MIIDATA_PACKET_SIZE (0x60)

typedef struct FFLStoreData
{
    union
    {
        u8 data[FFL_MIIDATA_PACKET_SIZE];
        u32 value32[FFL_MIIDATA_PACKET_SIZE / sizeof(u32)];
    };
}
FFLStoreData;

#ifdef __cplusplus
}
#endif
