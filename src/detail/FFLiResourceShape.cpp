#include <nn/ffl/detail/FFLiResourceShape.h>

namespace {

const void* GetElement(u32* pSize, const void* pShapeData, u32 pos, u32 size)
{
    *pSize = size;
    return size != 0 ? ((const u8*)pShapeData + pos) : NULL;
}

}

const void* FFLiGetResourceShapeElement(u32* pSize, const void* pShapeData, FFLiShapePartsType partsType, FFLiResourceShapeElementType elementType)
{
    const FFLiResourceShape* pShape = (const FFLiResourceShape*)pShapeData;

    switch (elementType)
    {
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION:
        return GetElement(pSize, pShape, pShape->dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION], pShape->dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION]);
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL:
        return GetElement(pSize, pShape, pShape->dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL], pShape->dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL]);
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD:
        return GetElement(pSize, pShape, pShape->dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD], pShape->dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD]);
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT:
        return GetElement(pSize, pShape, pShape->dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT], pShape->dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT]);
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR:
        return GetElement(pSize, pShape, pShape->dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR], pShape->dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR]);
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX:
        return GetElement(pSize, pShape, pShape->dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX], pShape->dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX]);
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_HAIR_1:
        if (partsType == FFLI_SHAPE_PARTS_TYPE_HAIR_1)
        {
            *pSize = sizeof(FFLVec3) * 6;
            return pShape->transform;
        }
        break;
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_FACELINE:
        if (partsType == FFLI_SHAPE_PARTS_TYPE_FACELINE)
        {
            *pSize = sizeof(FFLVec3) * 3;
            return pShape->transform;
        }
        break;
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BOUNDING_BOX:
        *pSize = sizeof(FFLBoundingBox);
        return &pShape->boundingBox;
    }

    *pSize = 0;
    return NULL;
}
