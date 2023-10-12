#ifndef FFLI_RESOURCE_PARTS_INFO_H_
#define FFLI_RESOURCE_PARTS_INFO_H_

#include <nn/ffl/types.h>

enum FFLiResourceWindowBits
{
    FFLI_RESOURCE_WINDOW_BITS_8     =  0,   // Window size = 0x0100 (256 B)
    FFLI_RESOURCE_WINDOW_BITS_9     =  1,   // Window size = 0x0200 (512 B)
    FFLI_RESOURCE_WINDOW_BITS_10    =  2,   // Window size = 0x0400 (1 KB)
    FFLI_RESOURCE_WINDOW_BITS_11    =  3,   // Window size = 0x0800 (2 KB)
    FFLI_RESOURCE_WINDOW_BITS_12    =  4,   // Window size = 0x1000 (4 KB)
    FFLI_RESOURCE_WINDOW_BITS_13    =  5,   // Window size = 0x2000 (8 KB)
    FFLI_RESOURCE_WINDOW_BITS_14    =  6,   // Window size = 0x4000 (16 KB)
    FFLI_RESOURCE_WINDOW_BITS_15    =  7,   // Window size = 0x8000 (32 KB)

    FFLI_RESOURCE_WINDOW_BITS_24    =  8,   // Window size = 0x01000000 (16 MB)
    FFLI_RESOURCE_WINDOW_BITS_25    =  9,   // Window size = 0x02000000 (32 MB)
    FFLI_RESOURCE_WINDOW_BITS_26    = 10,   // Window size = 0x04000000 (64 MB)
    FFLI_RESOURCE_WINDOW_BITS_27    = 11,   // Window size = 0x08000000 (128 MB)
    FFLI_RESOURCE_WINDOW_BITS_28    = 12,   // Window size = 0x10000000 (256 MB)
    FFLI_RESOURCE_WINDOW_BITS_29    = 13,   // Window size = 0x20000000 (512 MB)
    FFLI_RESOURCE_WINDOW_BITS_30    = 14,   // Window size = 0x40000000 (1 GB)
    FFLI_RESOURCE_WINDOW_BITS_31    = 15,   // Window size = 0x80000000 (2 GB)

    FFLI_RESOURCE_WINDOW_BITS_47    = 16,   // Window size = 0x0000800000000000 (128 TB)

    FFLI_RESOURCE_WINDOW_BITS_DEFAULT = FFLI_RESOURCE_WINDOW_BITS_15
};

enum FFLiResourceStrategy
{
    FFLI_RESOURCE_STRATEGY_UNCOMPRESSED = 5
};

struct FFLiResourcePartsInfo
{
    u32 dataPos;
    u32 dataSize;
    u32 compressedSize;
    u8  compressLevel;  // Maps to FFLiResourceCompressLevel; unused, but non-zero in files
    u8  windowBits;     // Maps to FFLiResourceWindowBits
    u8  memoryLevel;    // Maps to FFLiResourceMemoryLevel; unused, but non-zero in files
    u8  strategy;       // Maps to FFLiResourceStrategy; meaning only lies in whether the value is 5 or not
};
NN_STATIC_ASSERT(sizeof(FFLiResourcePartsInfo) == 0x10);

#endif // FFLI_RESOURCE_PARTS_INFO_H_
