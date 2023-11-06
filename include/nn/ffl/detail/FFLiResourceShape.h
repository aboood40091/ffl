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

const void* FFLiGetResourceShapeElement(u32* pSize, const void* pShapeData, FFLiShapePartsType partsType, FFLiResourceShapeElementType elementType);

void FFLiSwapEndianResourceShapeElement(void* pShapeData, FFLiShapePartsType partsType, bool save);

// --------------------------------------------------------------------------

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

#define FFLI_RESOURCE_SHAPE_TRANSFORM_MAX_SIZE  (0x48)

class FFLiResourceShapeDataHeader
{
public:
    u32 GetElementPos(FFLiResourceShapeElementType type) const
    {
        return m_ElementPos[type];
    }

    u32 GetElementSize(FFLiResourceShapeElementType type) const
    {
        return m_ElementSize[type];
    }

    FFLBoundingBox& GetBoundingBox()
    {
        return m_BoundingBox;
    }

    const FFLBoundingBox& GetBoundingBox() const
    {
        return m_BoundingBox;
    }

    void* GetTransform()
    {
        return m_Transform;
    }

    const void* GetTransform() const
    {
        return m_Transform;
    }

    void SwapEndian();  // Deleted in NSMBU

private:
    u32             m_ElementPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX];
    u32             m_ElementSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX];
    FFLBoundingBox  m_BoundingBox;
    u32             m_Transform[FFLI_RESOURCE_SHAPE_TRANSFORM_MAX_SIZE / sizeof(u32)];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceShapeDataHeader) == 0x90);

class FFLiResourceShapeHairTransform
{
public:
    const FFLVec3& Get(s32 i) const
    {
        return _0[i];
    }

    void SwapEndian();  // Deleted in NSMBU

private:
    FFLVec3 _0[6];
};
NN_STATIC_ASSERT(sizeof(FFLiResourceShapeHairTransform) == 0x48);

class FFLiResourceShapeFacelineTransform
{
public:
    const FFLVec3& GetHairPosition() const
    {
        return m_HairPos;
    }

    const FFLVec3& GetFaceCenterPosition() const
    {
        return m_FaceCenterPos;
    }

    const FFLVec3& GetBeardPosition() const
    {
        return m_BeardPos;
    }

    void SwapEndian();  // Deleted in NSMBU

private:
    FFLVec3 m_HairPos;
    FFLVec3 m_FaceCenterPos;
    FFLVec3 m_BeardPos;
};
NN_STATIC_ASSERT(sizeof(FFLiResourceShapeFacelineTransform) == 0x24);

NN_STATIC_ASSERT(sizeof(FFLiResourceShapeHairTransform) <= FFLI_RESOURCE_SHAPE_TRANSFORM_MAX_SIZE);
NN_STATIC_ASSERT(sizeof(FFLiResourceShapeFacelineTransform) <= FFLI_RESOURCE_SHAPE_TRANSFORM_MAX_SIZE);

#endif // FFLI_RESOURCE_SHAPE_H_
