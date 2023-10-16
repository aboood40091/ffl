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
    FFLiResourcePartsInfo   partsInfoCap1[FFL_HAIR_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoCap2[FFL_HAIR_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoFaceline[FFL_FACE_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoGlass[1];
    FFLiResourcePartsInfo   partsInfoMask[FFL_FACE_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoNoseline[FFL_NOSE_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoNose[FFL_NOSE_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoHair1[FFL_HAIR_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoHair2[FFL_HAIR_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoForehead1[FFL_HAIR_TYPE_MAX];
    FFLiResourcePartsInfo   partsInfoForehead2[FFL_HAIR_TYPE_MAX];
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
