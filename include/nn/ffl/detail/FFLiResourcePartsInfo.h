#ifndef FFLI_RESOURCE_PARTS_INFO_H_
#define FFLI_RESOURCE_PARTS_INFO_H_

#include <nn/ffl/types.h>

enum FFLiResourceCompressLevel
{
    FFLI_RESOURCE_COMPRESS_LEVEL_NO_COMPRESSION         = 0,
    FFLI_RESOURCE_COMPRESS_LEVEL_BEST_SPEED             = 1,
    FFLI_RESOURCE_COMPRESS_LEVEL_BEST_COMPRESSION       = 9,
    FFLI_RESOURCE_COMPRESS_LEVEL_DEFAULT_COMPRESSION    = 10,
    FFLI_RESOURCE_COMPRESS_LEVEL_MAX                    = 11
};

enum FFLiResourceWindowBits
{
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_8    =  0,   // Window size = 0x0100 (256 B)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_9    =  1,   // Window size = 0x0200 (512 B)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_10   =  2,   // Window size = 0x0400 (1 KB)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_11   =  3,   // Window size = 0x0800 (2 KB)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_12   =  4,   // Window size = 0x1000 (4 KB)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_13   =  5,   // Window size = 0x2000 (8 KB)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_14   =  6,   // Window size = 0x4000 (16 KB)
    FFLI_RESOURCE_WINDOW_BITS_ZLIB_15   =  7,   // Window size = 0x8000 (32 KB)

    FFLI_RESOURCE_WINDOW_BITS_GZIP_8    =  8,   // Window size = 0x0100 (256 B)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_9    =  9,   // Window size = 0x0200 (512 B)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_10   = 10,   // Window size = 0x0400 (1 KB)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_11   = 11,   // Window size = 0x0800 (2 KB)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_12   = 12,   // Window size = 0x1000 (4 KB)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_13   = 13,   // Window size = 0x2000 (8 KB)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_14   = 14,   // Window size = 0x4000 (16 KB)
    FFLI_RESOURCE_WINDOW_BITS_GZIP_15   = 15,   // Window size = 0x8000 (32 KB)

    FFLI_RESOURCE_WINDOW_BITS_ZLIB_OR_GZIP_15   = 16,   // Window size = 0x8000 (32 KB)

    FFLI_RESOURCE_WINDOW_BITS_MAX   = 17,   // Equivalent to FFLI_RESOURCE_WINDOW_BITS_ZLIB_15, although most likely unintentional
};

enum FFLiResourceMemoryLevel
{
    FFLI_RESOURCE_MEMORY_LEVEL_1    = 0,
    FFLI_RESOURCE_MEMORY_LEVEL_2    = 1,
    FFLI_RESOURCE_MEMORY_LEVEL_3    = 2,
    FFLI_RESOURCE_MEMORY_LEVEL_4    = 3,
    FFLI_RESOURCE_MEMORY_LEVEL_5    = 4,
    FFLI_RESOURCE_MEMORY_LEVEL_6    = 5,
    FFLI_RESOURCE_MEMORY_LEVEL_7    = 6,
    FFLI_RESOURCE_MEMORY_LEVEL_8    = 7,
    FFLI_RESOURCE_MEMORY_LEVEL_9    = 8,
    FFLI_RESOURCE_MEMORY_LEVEL_MAX  = 9
};

enum FFLiResourceStrategy
{
    FFLI_RESOURCE_STRATEGY_DEFAULT_STRATEGY = 0,
    FFLI_RESOURCE_STRATEGY_FILTERED         = 1,
    FFLI_RESOURCE_STRATEGY_HUFFMAN_ONLY     = 2,
    FFLI_RESOURCE_STRATEGY_RLE              = 3,
    FFLI_RESOURCE_STRATEGY_FIXED            = 4,
    FFLI_RESOURCE_STRATEGY_UNCOMPRESSED     = 5,
    FFLI_RESOURCE_STRATEGY_MAX              = 6
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
