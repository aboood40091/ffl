#include <nn/ffl/FFLiCharModel.h>
#include <nn/ffl/FFLiCoordinate.h>
#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiShape.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiBug.h>
#include <nn/ffl/detail/FFLiResourceShape.h>

#include <cstring>

union F32BitCast
{
    f32 f;
    u32 u;
    struct
    {
        u32 sign        : 1;    // (MSB)
        u32 exponent    : 8;
        u32 mantissa    : 23;   // (LSB)
    };
};
NN_STATIC_ASSERT(sizeof(F32BitCast) == 4);

static bool IsNaN(f32 value)
{
    F32BitCast x = { value };
    // Basically:
    // return x.exponent == 0xff && x.mantissa > 0;
    return (x.u << 1) > 0xff000000;
}

namespace {

FFLiResourceShapeElementType GetElementType(FFLAttributeBufferType type);

u32 GetStride(FFLAttributeBufferType type, u32 size);

void EndianSwap(void* ptr, u32 size);

template <typename T>
void AdjustAttribute(T* pVec, u32 num, f32 scaleX, f32 scaleY, f32 scaleZ, const FFLVec3* pTranslate, bool flipX, const FFLiCoordinate* pCoordinate);

template <typename T>
void AdjustAttributeWithoutScale(T* pVec, u32 num, bool flipX, const FFLiCoordinate* pCoordinate);

}

bool FFLiCanDrawShape(const FFLDrawParam* pDrawParam)
{
    return pDrawParam->primitiveParam.indexCount != 0;
}

FFLResult FFLiLoadShape(FFLDrawParam* pDrawParam, FFLBoundingBox* pBoundingBox, FFLiCharModel* pModel, FFLiShapePartsType partsType, u16 index, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator)
{
    u32 size = pResLoader->GetShapeAlignedMaxSize(partsType);
    void* pData = pAllocator->Allocate(size, FS_IO_BUFFER_ALIGN);

    FFLResult result = pResLoader->LoadShape(pData, &size, partsType, index);
    if (result != FFL_RESULT_OK)
        return result;

    if (size == 0)
    {
        std::memset(pDrawParam, 0, sizeof(FFLDrawParam));

        const u32 NaN = 0x7FC00000;
        static const u32 INVALID_BOUNDING_BOX[2][3] = {
            { NaN, NaN, NaN },
            { NaN, NaN, NaN }
        };
        NN_STATIC_ASSERT(sizeof(INVALID_BOUNDING_BOX) == sizeof(FFLBoundingBox));
        *pBoundingBox = *(FFLBoundingBox*)&INVALID_BOUNDING_BOX;
    }
    else
    {
        for (u32 i = 0; i < FFL_ATTRIBUTE_BUFFER_TYPE_MAX; i++)
        {
            FFLiResourceShapeElementType elementType = GetElementType(FFLAttributeBufferType(i));
            FFLAttributeBuffer& attribute = pDrawParam->attributeBufferParam.attributeBuffers[i];
            attribute.ptr = const_cast<void*>(FFLiGetResourceShapeElement(&attribute.size, pData, partsType, elementType));
            attribute.stride = GetStride(FFLAttributeBufferType(i), attribute.size);
        }

        pDrawParam->primitiveParam.pIndexBuffer = const_cast<void*>(FFLiGetResourceShapeElement(&pDrawParam->primitiveParam.indexCount, pData, partsType, FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX));
        pDrawParam->primitiveParam.indexFormat = GX2_INDEX_FORMAT_U16;
        pDrawParam->primitiveParam.primitiveType = GX2_PRIMITIVE_TRIANGLES;

        if (partsType == FFLI_SHAPE_PARTS_TYPE_HAIR_1)
        {
            const FFLiResourceShapeHairTransform* pTransform = static_cast<const FFLiResourceShapeHairTransform*>(FFLiGetResourceShapeElement(&size, pData, partsType, FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_HAIR_1));

            pModel->partsTransform._18 = pTransform->Get(0);
            pModel->partsTransform._c  = pTransform->Get(1);
            pModel->partsTransform._30 = pTransform->Get(2);
            pModel->partsTransform._24 = pTransform->Get(3);
            pModel->partsTransform._48 = pTransform->Get(4);
            pModel->partsTransform._3c = pTransform->Get(5);
        }
        else if (partsType == FFLI_SHAPE_PARTS_TYPE_FACELINE)
        {
            const FFLiResourceShapeFacelineTransform* pTransform = static_cast<const FFLiResourceShapeFacelineTransform*>(FFLiGetResourceShapeElement(&size, pData, partsType, FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TRANSFORM_FACELINE));

            pModel->beardPos        = pTransform->GetBeardPosition();
            pModel->hairPos         = pTransform->GetHairPosition();
            pModel->faceCenterPos   = pTransform->GetFaceCenterPosition();

            pModel->partsTransform._0 = pTransform->GetHairPosition();
        }

        std::memcpy(pBoundingBox, FFLiGetResourceShapeElement(&size, pData, partsType, FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BOUNDING_BOX), sizeof(FFLBoundingBox));
    }

    return FFL_RESULT_OK;
}

void FFLiAdjustShape(FFLDrawParam* pDrawParam, FFLBoundingBox* pBoundingBox, f32 scaleX, f32 scaleY, const FFLVec3* pTranslate, bool flipX, const FFLiCoordinate* pCoordinate, FFLiShapePartsType partsType, bool limitNoseScaleZ)
{
    f32 scaleZ = (scaleX + scaleY) * 0.5f;
    if (limitNoseScaleZ &&
        (partsType == FFLI_SHAPE_PARTS_TYPE_NOSE || partsType == FFLI_SHAPE_PARTS_TYPE_NOSELINE) &&
        scaleZ > 1.1f)
    {
        scaleZ = 1.1f;
    }

    AdjustAttribute<FFLVec4>(
        static_cast<FFLVec4*>(pDrawParam->attributeBufferParam.attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr),
        pDrawParam->attributeBufferParam.attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].size / sizeof(FFLVec4),
        scaleX, scaleY, scaleZ,
        pTranslate,
        flipX,
        pCoordinate
    );

    AdjustAttributeWithoutScale<FFLiSnorm10_10_10_2>(
        static_cast<FFLiSnorm10_10_10_2*>(pDrawParam->attributeBufferParam.attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL].ptr),
        pDrawParam->attributeBufferParam.attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL].size / sizeof(FFLiSnorm10_10_10_2),
        flipX,
        pCoordinate
    );

    AdjustAttributeWithoutScale<FFLiSnorm8_8_8_8>(
        static_cast<FFLiSnorm8_8_8_8*>(pDrawParam->attributeBufferParam.attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT].ptr),
        pDrawParam->attributeBufferParam.attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT].size / sizeof(FFLiSnorm8_8_8_8),
        flipX,
        pCoordinate
    );

    if (!IsNaN(pBoundingBox->min.x))
        AdjustAttribute<FFLVec3>(
            &pBoundingBox->min,
            2,
            scaleX, scaleY, scaleZ,
            pTranslate,
            flipX,
            pCoordinate
        );

    for (u32 i = 0; i < FFL_ATTRIBUTE_BUFFER_TYPE_MAX; i++)
    {
        FFLAttributeBuffer& attribute = pDrawParam->attributeBufferParam.attributeBuffers[i];
        if (attribute.ptr != NULL)
            EndianSwap(attribute.ptr, attribute.size);
    }

    FFLPrimitiveParam& primitive = pDrawParam->primitiveParam;
    if (primitive.pIndexBuffer != NULL)
        EndianSwap(
            primitive.pIndexBuffer,
            sizeof(u16) * primitive.indexCount  // Apparently Nintendo forgot the index count is 4
        );
}

void FFLiInvalidateShape(FFLDrawParam* pDrawParam)
{
    for (u32 i = 0; i < FFL_ATTRIBUTE_BUFFER_TYPE_MAX; i++)
    {
        FFLAttributeBuffer& attribute = pDrawParam->attributeBufferParam.attributeBuffers[i];
        if (attribute.ptr != NULL)
            FFLiInvalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, attribute.ptr, attribute.size);
    }

    FFLPrimitiveParam& primitive = pDrawParam->primitiveParam;
    if (primitive.pIndexBuffer != NULL)
        FFLiInvalidate(
            GX2_INVALIDATE_CPU_ATTRIB_BUFFER,
            primitive.pIndexBuffer,
            sizeof(u16) * primitive.indexCount  // Apparently Nintendo forgot the index count is 4
        );
}

namespace {

FFLiResourceShapeElementType GetElementType(FFLAttributeBufferType type)
{
    switch (type)
    {
    case FFL_ATTRIBUTE_BUFFER_TYPE_POSITION:
        return FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION;
    case FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD:
        return FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD;
    case FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL:
        return FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL;
    case FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT:
        return FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT;
    case FFL_ATTRIBUTE_BUFFER_TYPE_COLOR:
        return FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR;
    }
    return FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION;
}

u32 FormatToStride(GX2AttribFormat format)
{
    switch (format)
    {
    case GX2_ATTRIB_FORMAT_32_32_FLOAT:
        return 8;
    case GX2_ATTRIB_FORMAT_32_32_32_FLOAT:
    case GX2_ATTRIB_FORMAT_32_32_32_32_FLOAT:
        return 16;
    case GX2_ATTRIB_FORMAT_8_8_8_8_UNORM:
    case GX2_ATTRIB_FORMAT_8_8_8_8_SNORM:
    case GX2_ATTRIB_FORMAT_10_10_10_2_SNORM:
        return 4;
    }
    return 0;
}

u32 GetStride(FFLAttributeBufferType type, u32 size)
{
    u32 stride = 0;

    switch (type)
    {
    case FFL_ATTRIBUTE_BUFFER_TYPE_POSITION:
        stride = FormatToStride(GX2_ATTRIB_FORMAT_32_32_32_FLOAT);
        break;
    case FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD:
        stride = FormatToStride(GX2_ATTRIB_FORMAT_32_32_FLOAT);
        break;
    case FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL:
        stride = FormatToStride(GX2_ATTRIB_FORMAT_10_10_10_2_SNORM);
        break;
    case FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT:
        stride = FormatToStride(GX2_ATTRIB_FORMAT_8_8_8_8_SNORM);
        break;
    case FFL_ATTRIBUTE_BUFFER_TYPE_COLOR:
        stride = FormatToStride(GX2_ATTRIB_FORMAT_8_8_8_8_UNORM);
        break;
    }

    if (stride >= size)
        stride = 0;
    
    return stride;
}

void EndianSwap(void* ptr, u32 size)
{
    FFLiBugEndianSwap(ptr, size);
}

template <typename T>
void AdjustAttribute(T* pVec, u32 num, f32 scaleX, f32 scaleY, f32 scaleZ, const FFLVec3* pTranslate, bool flipX, const FFLiCoordinate* pCoordinate)
{
    bool isDefault = pCoordinate->IsDefault();

    for (u32 i = 0; i < num; i++)
    {
        if (flipX)
            pVec->x = FFLiCoordinate::FlipValue(pVec, pVec->x);

        if (scaleX != 1.0f)
            pVec->x *= scaleX;

        if (scaleY != 1.0f)
            pVec->y *= scaleY;

        if (scaleZ != 1.0f)
            pVec->z *= scaleZ;

        if (pTranslate != NULL)
        {
            pVec->x += pTranslate->x;
            pVec->y += pTranslate->y;
            pVec->z += pTranslate->z;
        }

        if (!isDefault)
            pCoordinate->Transform(pVec);

        pVec++;
    }
}

template <typename T>
void AdjustAttributeWithoutScale(T* pVec, u32 num, bool flipX, const FFLiCoordinate* pCoordinate)
{
    bool isDefault = pCoordinate->IsDefault();

    for (u32 i = 0; i < num; i++)
    {
        T& vec = pVec[i];

        if (flipX)
            vec.x = FFLiCoordinate::FlipValue(&vec, vec.x);

        if (!isDefault)
            pCoordinate->TransformWithoutScale(&vec);
    }
}

}
