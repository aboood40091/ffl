#include <nn/ffl/FFLiResourceHeader.h>
#include <nn/ffl/FFLiSwapEndian.h>
#include <nn/ffl/FFLiUtil.h>

#define SIZE_OF_MEMBER(cls, member) sizeof( ((cls*)0)->member )

u32 FFLiGetTextureResourceNum(FFLiTexturePartsType partsType)
{
    switch (partsType)
    {
    case FFLI_TEXTURE_PARTS_TYPE_BEARD:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoBeard) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_CAP:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoCap) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_EYE:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoEye) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_EYEBROW:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoEyebrow) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_FACELINE:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoFaceline) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_FACE_MAKEUP:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoFaceMakeup) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_GLASS:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoGlass) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_MOLE:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoMole) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_MOUTH:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoMouth) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_MUSTACHE:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoMustache) / sizeof(FFLiResourcePartsInfo);
    case FFLI_TEXTURE_PARTS_TYPE_NOSELINE:
        return SIZE_OF_MEMBER(FFLiResourceTextureHeader, partsInfoNoseline) / sizeof(FFLiResourcePartsInfo);
    default:
        return 0;
    }
}

u32 FFLiGetShapeResourceNum(FFLiShapePartsType partsType)
{
    switch (partsType)
    {
    case FFLI_SHAPE_PARTS_TYPE_BEARD:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoBeard) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_CAP_1:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoCap1) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_CAP_2:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoCap2) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_FACELINE:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoFaceline) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_GLASS:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoGlass) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_MASK:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoMask) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_NOSELINE:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoNoseline) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_NOSE:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoNose) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_HAIR_1:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoHair1) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_HAIR_2:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoHair2) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_FOREHEAD_1:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoForehead1) / sizeof(FFLiResourcePartsInfo);
    case FFLI_SHAPE_PARTS_TYPE_FOREHEAD_2:
        return SIZE_OF_MEMBER(FFLiResourceShapeHeader, partsInfoForehead2) / sizeof(FFLiResourcePartsInfo);
    default:
        return 0;
    }
}

FFLiResourcePartsInfo* FFLiGetTextureResoucePartsInfos(u32* pNum, FFLiResourceTextureHeader* pHeader, FFLiTexturePartsType partsType)
{
    *pNum = FFLiGetTextureResourceNum(partsType);
    switch (partsType)
    {
    case FFLI_TEXTURE_PARTS_TYPE_BEARD:
        return pHeader->partsInfoBeard;
    case FFLI_TEXTURE_PARTS_TYPE_CAP:
        return pHeader->partsInfoCap;
    case FFLI_TEXTURE_PARTS_TYPE_EYE:
        return pHeader->partsInfoEye;
    case FFLI_TEXTURE_PARTS_TYPE_EYEBROW:
        return pHeader->partsInfoEyebrow;
    case FFLI_TEXTURE_PARTS_TYPE_FACELINE:
        return pHeader->partsInfoFaceline;
    case FFLI_TEXTURE_PARTS_TYPE_FACE_MAKEUP:
        return pHeader->partsInfoFaceMakeup;
    case FFLI_TEXTURE_PARTS_TYPE_GLASS:
        return pHeader->partsInfoGlass;
    case FFLI_TEXTURE_PARTS_TYPE_MOLE:
        return pHeader->partsInfoMole;
    case FFLI_TEXTURE_PARTS_TYPE_MOUTH:
        return pHeader->partsInfoMouth;
    case FFLI_TEXTURE_PARTS_TYPE_MUSTACHE:
        return pHeader->partsInfoMustache;
    case FFLI_TEXTURE_PARTS_TYPE_NOSELINE:
        return pHeader->partsInfoNoseline;
    default:
        return NULL;
    }
}

FFLiResourcePartsInfo* FFLiGetShapeResoucePartsInfos(u32* pNum, FFLiResourceShapeHeader* pHeader, FFLiShapePartsType partsType)
{
    *pNum = FFLiGetShapeResourceNum(partsType);
    switch (partsType)
    {
    case FFLI_SHAPE_PARTS_TYPE_BEARD:
        return pHeader->partsInfoBeard;
    case FFLI_SHAPE_PARTS_TYPE_CAP_1:
        return pHeader->partsInfoCap1;
    case FFLI_SHAPE_PARTS_TYPE_CAP_2:
        return pHeader->partsInfoCap2;
    case FFLI_SHAPE_PARTS_TYPE_FACELINE:
        return pHeader->partsInfoFaceline;
    case FFLI_SHAPE_PARTS_TYPE_GLASS:
        return pHeader->partsInfoGlass;
    case FFLI_SHAPE_PARTS_TYPE_MASK:
        return pHeader->partsInfoMask;
    case FFLI_SHAPE_PARTS_TYPE_NOSELINE:
        return pHeader->partsInfoNoseline;
    case FFLI_SHAPE_PARTS_TYPE_NOSE:
        return pHeader->partsInfoNose;
    case FFLI_SHAPE_PARTS_TYPE_HAIR_1:
        return pHeader->partsInfoHair1;
    case FFLI_SHAPE_PARTS_TYPE_HAIR_2:
        return pHeader->partsInfoHair2;
    case FFLI_SHAPE_PARTS_TYPE_FOREHEAD_1:
        return pHeader->partsInfoForehead1;
    case FFLI_SHAPE_PARTS_TYPE_FOREHEAD_2:
        return pHeader->partsInfoForehead2;
    default:
        return NULL;
    }
}

s32 FFLiResourceWindowBitsToZlibWindowBits(FFLiResourceWindowBits windowBits)
{
    switch (windowBits)
    {
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_8:
        return 8;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_9:
        return 9;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_10:
        return 10;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_11:
        return 11;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_12:
        return 12;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_13:
        return 13;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_14:
        return 14;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_15:
        return 15;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_8:
        return 16 + 8;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_9:
        return 16 + 9;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_10:
        return 16 + 10;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_11:
        return 16 + 11;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_12:
        return 16 + 12;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_13:
        return 16 + 13;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_14:
        return 16 + 14;
    case FFLI_RESOURCE_WINDOW_BITS_GZIP_15:
        return 16 + 15;
    case FFLI_RESOURCE_WINDOW_BITS_ZLIB_OR_GZIP_15:
        return 32 + 15;
    default:
        return 15;
    }
}

FFLResult FFLiResourceHeader::GetResult() const
{
    if (!FFLiCheckAlignPtr(this, 4))
        return FFL_RESULT_ERROR;

    if (m_Magic != 0x46465241)  // FFRA
        return FFL_RESULT_FILE_INVALID;

    if (m_Version != FFLI_RESOURCE_HEADER_VERSION)
        return FFL_RESULT_FILE_INVALID;

    return FFL_RESULT_OK;
}

namespace {

void SwapEndianResourcePartsInfo(FFLiResourcePartsInfo* pPartsInfo, u32 num)
{
    // This function is deleted in NSMBU.
    // Therefore, its implementation is only theoretical.

    for (u32 i = 0; i < num; i++)
    {
        pPartsInfo[i].dataPos = FFLiSwapEndianImpl<u32>(pPartsInfo[i].dataPos);
        pPartsInfo[i].dataSize = FFLiSwapEndianImpl<u32>(pPartsInfo[i].dataSize);
        pPartsInfo[i].compressedSize = FFLiSwapEndianImpl<u32>(pPartsInfo[i].compressedSize);
    }
}

}

void FFLiResourceHeader::SwapEndian()
{
    // This function is deleted in NSMBU.
    // Therefore, its implementation is only theoretical.

    m_Magic = FFLiSwapEndianImpl<u32>(m_Magic);
    m_Version = FFLiSwapEndianImpl<u32>(m_Version);
    m_UncompressBufferSize = FFLiSwapEndianImpl<u32>(m_UncompressBufferSize);

    // Dunno what to do with this
    // _c

    m_IsExpand = FFLiSwapEndianImpl<u32>(m_IsExpand);

    for (u32 i = 0; i < FFLI_TEXTURE_PARTS_TYPE_MAX; i++)
    {
        u32 num;
        FFLiResourcePartsInfo* pPartsInfo = FFLiGetTextureResoucePartsInfos(&num, GetTextureHeader(), FFLiTexturePartsType(i));
        SwapEndianResourcePartsInfo(pPartsInfo, num);
    }

    for (u32 i = 0; i < FFLI_SHAPE_PARTS_TYPE_MAX; i++)
    {
        u32 num;
        FFLiResourcePartsInfo* pPartsInfo = FFLiGetShapeResoucePartsInfos(&num, GetShapeHeader(), FFLiShapePartsType(i));
        SwapEndianResourcePartsInfo(pPartsInfo, num);
    }

    // Dunno what to do with this
    // _49d0
}
