#include <nn/ffl/FFLiResourceHeader.h>
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
    }
    return 0;
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
    }
    return 0;
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
    }
    return NULL;
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
    }
    return NULL;
}

s32 FFLiResourceWindowBitsToZlibWindowBits(FFLiResourceWindowBits windowBits)
{
    switch (windowBits)
    {
    case FFLI_RESOURCE_WINDOW_BITS_8:
        return 8;
    case FFLI_RESOURCE_WINDOW_BITS_9:
        return 9;
    case FFLI_RESOURCE_WINDOW_BITS_10:
        return 10;
    case FFLI_RESOURCE_WINDOW_BITS_11:
        return 11;
    case FFLI_RESOURCE_WINDOW_BITS_12:
        return 12;
    case FFLI_RESOURCE_WINDOW_BITS_13:
        return 13;
    case FFLI_RESOURCE_WINDOW_BITS_14:
        return 14;
    case FFLI_RESOURCE_WINDOW_BITS_15:
        return 15;
    case FFLI_RESOURCE_WINDOW_BITS_24:
        return 24;
    case FFLI_RESOURCE_WINDOW_BITS_25:
        return 25;
    case FFLI_RESOURCE_WINDOW_BITS_26:
        return 26;
    case FFLI_RESOURCE_WINDOW_BITS_27:
        return 27;
    case FFLI_RESOURCE_WINDOW_BITS_28:
        return 28;
    case FFLI_RESOURCE_WINDOW_BITS_29:
        return 29;
    case FFLI_RESOURCE_WINDOW_BITS_30:
        return 30;
    case FFLI_RESOURCE_WINDOW_BITS_31:
        return 31;
    case FFLI_RESOURCE_WINDOW_BITS_47:
        return 47;
    }
    return 15;
}

FFLResult FFLiResourceHeader::GetResult() const
{
    if (!FFLiCheckAlignPtr(this, 4))
        return FFL_RESULT_ERROR;

    if (m_Magic != 'FFRA')
        return FFL_RESULT_FILE_INVALID;

    if (m_Version != FFLI_RESOURCE_HEADER_VERSION)
        return FFL_RESULT_FILE_INVALID;

    return FFL_RESULT_OK;
}
