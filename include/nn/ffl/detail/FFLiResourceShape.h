#ifndef FFLI_RESOURCE_SHAPE_H_
#define FFLI_RESOURCE_SHAPE_H_

#include <nn/ffl/FFLBoundingBox.h>

#include <nn/ffl/FFLiShapePartsType.h>

#include <nn/ffl/detail/FFLiResourcePartsInfo.h>

enum FFLiResourceShapeElementType
{
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION   = 0,
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL     = 1,
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD   = 2,
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT    = 3,
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR      = 4,
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX      = 5,

    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_HAIR_1   = 6,
    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_FACELINE = 7,

    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BOUNDING_BOX   = 8,

    FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX = FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX + 1
};

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

struct FFLiResourceShape
{
    u32             dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX];
    u32             dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX];
    FFLBoundingBox  boundingBox;
    FFLVec3         transform[6];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceShape) == 0x90);

const void* FFLiGetResourceShapeElement(u32* pSize, const void* pShapeData, FFLiShapePartsType partsType, FFLiResourceShapeElementType elementType);

#endif // FFLI_RESOURCE_SHAPE_H_
