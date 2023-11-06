#include <nn/ffl/FFLiSwapEndian.h>

#include <nn/ffl/detail/FFLiResourceShape.h>

namespace {

const void* GetElement(u32* pSize, const void* pShapeData, u32 pos, u32 size);

void SwapEndianAttribute(void* pShapeData, u32 pos, u32 size);
void SwapEndianIndex(void* pShapeData, u32 pos, u32 size);
void SwapEndianVec3(FFLVec3* pVec);
void SwapEndianHairTransform(void* pTransformData);
void SwapEndianFacelineTransform(void* pTransformData);

}

const void* FFLiGetResourceShapeElement(u32* pSize, const void* pShapeData, FFLiShapePartsType partsType, FFLiResourceShapeElementType elementType)
{
    const FFLiResourceShapeDataHeader* pShape = (const FFLiResourceShapeDataHeader*)pShapeData;

    switch (elementType)
    {
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION:
        return GetElement(pSize, pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION));
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL:
        return GetElement(pSize, pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL));
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD:
        return GetElement(pSize, pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD));
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT:
        return GetElement(pSize, pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT));
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR:
        return GetElement(pSize, pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR));
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX:
        return GetElement(pSize, pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX));
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_HAIR_1:
        if (partsType == FFLI_SHAPE_PARTS_TYPE_HAIR_1)
        {
            *pSize = sizeof(FFLiResourceShapeHairTransform);
            return pShape->GetTransform();
        }
        break;
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_FACELINE:
        if (partsType == FFLI_SHAPE_PARTS_TYPE_FACELINE)
        {
            *pSize = sizeof(FFLiResourceShapeFacelineTransform);
            return pShape->GetTransform();
        }
        break;
    case FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BOUNDING_BOX:
        *pSize = sizeof(FFLBoundingBox);
        return &pShape->GetBoundingBox();
    }

    *pSize = 0;
    return NULL;
}

void FFLiSwapEndianResourceShapeElement(void* pShapeData, FFLiShapePartsType partsType, bool save)
{
    // This function is deleted in NSMBU.
    // Therefore, its implementation is only theoretical.

    FFLiResourceShapeDataHeader* pShape = static_cast<FFLiResourceShapeDataHeader*>(pShapeData);

    if (!save)
        pShape->SwapEndian();

    SwapEndianAttribute(pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION));
    SwapEndianAttribute(pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL));
    SwapEndianAttribute(pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD));
    // Tangent and color don't need endianness swap

    SwapEndianIndex(pShape, pShape->GetElementPos(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX), pShape->GetElementSize(FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX));

    SwapEndianVec3(&(pShape->GetBoundingBox().min));
    SwapEndianVec3(&(pShape->GetBoundingBox().max));

    if (partsType == FFLI_SHAPE_PARTS_TYPE_HAIR_1)
        SwapEndianHairTransform(pShape->GetTransform());

    else if (partsType == FFLI_SHAPE_PARTS_TYPE_FACELINE)
        SwapEndianFacelineTransform(pShape->GetTransform());

    if (save)
        pShape->SwapEndian();
}

void FFLiResourceShapeDataHeader::SwapEndian()
{
    FFLiSwapEndianArrayImpl<u32>(m_ElementPos, FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX);
    FFLiSwapEndianArrayImpl<u32>(m_ElementSize, FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX);
}

void FFLiResourceShapeHairTransform::SwapEndian()
{
    SwapEndianVec3(&(_0[0]));
    SwapEndianVec3(&(_0[1]));
    SwapEndianVec3(&(_0[2]));
    SwapEndianVec3(&(_0[3]));
    SwapEndianVec3(&(_0[4]));
    SwapEndianVec3(&(_0[5]));
}

void FFLiResourceShapeFacelineTransform::SwapEndian()
{
    SwapEndianVec3(&m_HairPos);
    SwapEndianVec3(&m_FaceCenterPos);
    SwapEndianVec3(&m_BeardPos);
}

namespace {

const void* GetElement(u32* pSize, const void* pShapeData, u32 pos, u32 size)
{
    *pSize = size;
    return size != 0 ? ((const u8*)pShapeData + pos) : NULL;
}

// These functions are all deleted in NSMBU.
// Therefore, their implementation is only theoretical.

void SwapEndianAttribute(void* pShapeData, u32 pos, u32 size)
{
    if (size == 0)
        return;

    FFLiSwapEndianArrayImpl<u32>((u32*)((const u8*)pShapeData + pos), size / sizeof(u32));
}

void SwapEndianIndex(void* pShapeData, u32 pos, u32 size)
{
    if (size == 0)
        return;

    FFLiSwapEndianArrayImpl<u16>((u16*)((const u8*)pShapeData + pos), size);
}

void SwapEndianVec3(FFLVec3* pVec)
{
    FFLiSwapEndianArrayImpl<f32>(&pVec->x, sizeof(FFLVec3) / sizeof(f32));
}

void SwapEndianHairTransform(void* pTransformData)
{
    static_cast<FFLiResourceShapeHairTransform*>(pTransformData)->SwapEndian();
}

void SwapEndianFacelineTransform(void* pTransformData)
{
    static_cast<FFLiResourceShapeFacelineTransform*>(pTransformData)->SwapEndian();
}

}
