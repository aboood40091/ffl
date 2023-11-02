#ifndef FFLI_RESOURCE_HEADER_H_
#define FFLI_RESOURCE_HEADER_H_

#include <nn/ffl/FFLResourceType.h>
#include <nn/ffl/FFLResult.h>

#include <nn/ffl/FFLiShapePartsType.h>
#include <nn/ffl/FFLiTexturePartsType.h>

#include <nn/ffl/detail/FFLiResourceShape.h>
#include <nn/ffl/detail/FFLiResourceTexture.h>

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

#define FFLI_RESOURCE_HEADER_VERSION    (0x00070000)

class FFLiResourceHeader
{
public:
    FFLResult GetResult() const;

    u32 GetUncompressBufferSize() const
    {
        return m_UncompressBufferSize;
    }

    bool IsExpand() const
    {
        return m_IsExpand == TRUE;
    }

    FFLiResourceTextureHeader* GetTextureHeader()
    {
        return &m_TextureHeader;
    }

    u32 GetTextureMaxSize(FFLiTexturePartsType partsType) const
    {
        return m_TextureHeader.partsMaxSize[partsType];
    }

    FFLiResourceShapeHeader* GetShapeHeader()
    {
        return &m_ShapeHeader;
    }

    u32 GetShapeMaxSize(FFLiShapePartsType partsType) const
    {
        return m_ShapeHeader.partsMaxSize[partsType];
    }

    void SwapEndian();  // Deleted in NSMBU

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
NN_STATIC_ASSERT_IS_POD(FFLiResourceHeader);
NN_STATIC_ASSERT(sizeof(FFLiResourceHeader) == 0x4A00);

struct FFLiResourceMultiHeader
{
    FFLiResourceHeader  header[FFL_RESOURCE_TYPE_MAX];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceMultiHeader) == 0x9400);

#endif // FFLI_RESOURCE_HEADER_H_
