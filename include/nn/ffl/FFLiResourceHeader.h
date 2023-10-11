#ifndef FFLI_RESOURCE_HEADER_H_
#define FFLI_RESOURCE_HEADER_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiShapePartsType.h>
#include <nn/ffl/FFLiTexturePartsType.h>

struct  FFLiResourcePartsInfo;
struct  FFLiResourceShapeHeader;
struct  FFLiResourceTextureHeader;
enum    FFLiResourceWindowBits;

u32 FFLiGetTextureResourceNum(FFLiTexturePartsType partsType);
u32 FFLiGetShapeResourceNum(FFLiShapePartsType partsType);

FFLiResourcePartsInfo* FFLiGetTextureResoucePartsInfos(u32* pNum, FFLiResourceTextureHeader* pHeader, FFLiTexturePartsType partsType);
FFLiResourcePartsInfo* FFLiGetShapeResoucePartsInfos(u32* pNum, FFLiResourceShapeHeader* pHeader, FFLiShapePartsType partsType);

s32 FFLiResourceWindowBitsToZlibWindowBits(FFLiResourceWindowBits windowBits);

// --------------------------------------------------------------------------

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

struct FFLiResourceTextureHeader
{
    u32                     partsMaxSize[FFLI_TEXTURE_PARTS_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoBeard[3];
    FFLiResourcePartsInfo   partsInfoCap[132];
    FFLiResourcePartsInfo   partsInfoEye[62];
    FFLiResourcePartsInfo   partsInfoEyebrow[24];
    FFLiResourcePartsInfo   partsInfoFaceline[12];
    FFLiResourcePartsInfo   partsInfoFaceMakeup[12];
    FFLiResourcePartsInfo   partsInfoGlass[9];
    FFLiResourcePartsInfo   partsInfoMole[2];
    FFLiResourcePartsInfo   partsInfoMouth[37];
    FFLiResourcePartsInfo   partsInfoMustache[6];
    FFLiResourcePartsInfo   partsInfoNoseline[18];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceTextureHeader) == 0x13FC);

struct FFLiResourceTextureFooter
{
    s32 mipOffset;
    u16 width;
    u16 height;
    u8  numMips;
    u8  format;     // Maps to FFLiTextureFormat
};
NN_STATIC_ASSERT(sizeof(FFLiResourceTextureFooter) == 0xC);

struct FFLiResourceShapeHeader
{
    u32                     partsMaxSize[FFLI_SHAPE_PARTS_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoBeard[4];
    FFLiResourcePartsInfo   partsInfoCap1[132];
    FFLiResourcePartsInfo   partsInfoCap2[132];
    FFLiResourcePartsInfo   partsInfoFaceline[12];
    FFLiResourcePartsInfo   partsInfoGlass[1];
    FFLiResourcePartsInfo   partsInfoMask[12];
    FFLiResourcePartsInfo   partsInfoNoseline[18];
    FFLiResourcePartsInfo   partsInfoNose[18];
    FFLiResourcePartsInfo   partsInfoHair1[132];
    FFLiResourcePartsInfo   partsInfoHair2[132];
    FFLiResourcePartsInfo   partsInfoForehead1[132];
    FFLiResourcePartsInfo   partsInfoForehead2[132];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceShapeHeader) == 0x35C0);

#define FFLI_RESOURCE_HEADER_VERSION    (0x00070000)

class FFLiResourceHeader
{
public:
    FFLResult GetResult() const;

private:
    u32                         m_Magic;
    u32                         m_Version;                  // I think? Always 0x00070000
    u32                         m_UncompressBufferSize;
    u32                         _c[4 / sizeof(u32)];        // Unused, but non-zero in files
    BOOL                        m_IsExpand;                 // Must be 1 specifically to be true, not just non-zero
    FFLiResourceTextureHeader   m_TextureHeader;
    FFLiResourceShapeHeader     m_ShapeHeader;
    u32                         _49d0[48 / sizeof(u32)];    // Unused AND zero in files
};
NN_STATIC_ASSERT(sizeof(BOOL) == 4);
NN_STATIC_ASSERT(sizeof(FFLiResourceHeader) == 0x4A00);

struct FFLiResourceMultiHeader
{
    FFLiResourceHeader  header[FFL_RESOURCE_TYPE_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceMultiHeader) == 0x9400);

#endif // FFLI_RESOURCE_HEADER_H_
