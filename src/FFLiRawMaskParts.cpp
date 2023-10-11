#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiRawMaskParts.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiBug.h>

#include <cstring>

namespace {

void CalcMVMatrix(Mat34* pMVMatrix, const FFLiRawMaskPartsDesc* pDesc);

void InitPrimitive(FFLPrimitiveParam* pPrimitive, FFLiBufferAllocator* pAllocator);
void InitAttributes(FFLAttributeBufferParam* pAttributes, FFLiOriginPosition originPosition, FFLiBufferAllocator* pAllocator, const Mat44* pMVPMatrix);

void InvalidatePrimitive(FFLPrimitiveParam* pPrimitive);
void InvalidateAttributes(FFLAttributeBufferParam* pAttributes);

void InitAttributesForFill(FFLAttributeBufferParam* pAttributes, FFLiBufferAllocator* pAllocator);

}

u32 FFLiGetBufferRawMaskParts()
{
    const u32 INDEX_BUFFER_SIZE = sizeof(u16) * 4;
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec4) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    u32 ret  = GX2_VERTEX_BUFFER_ALIGNMENT; // max(GX2_INDEX_BUFFER_ALIGNMENT, GX2_VERTEX_BUFFER_ALIGNMENT)
    ret     += FFLiRoundUp(FFLiBugCanVgtFixedIndexSize(FFLiBugCanSwapSize(INDEX_BUFFER_SIZE)), GX2_INDEX_BUFFER_ALIGNMENT);
    ret     += FFLiRoundUp(FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), GX2_VERTEX_BUFFER_ALIGNMENT);
    ret     += FFLiRoundUp(FFLiBugCanSwapSize(TEXCOORD_BUFFER_SIZE), GX2_VERTEX_BUFFER_ALIGNMENT);
    return ret;
}

void FFLiInitDrawParamRawMaskParts(FFLiRawMaskPartsDrawParam* pDrawParam, const FFLiRawMaskPartsDesc* pDesc, const Mat44* pProjMatrix, FFLiBufferAllocator* pAllocator)
{
    Mat44 mvpMatrix;
    Mat34 mvMatrix;

    CalcMVMatrix(&mvMatrix, pDesc);
    MAT34To44(&mvMatrix, &mvpMatrix);
    MAT44Concat(pProjMatrix, &mvpMatrix, &mvpMatrix);

    InitPrimitive(&pDrawParam->primitiveParam, pAllocator);
    InitAttributes(&pDrawParam->attributeBufferParam, pDesc->originPos, pAllocator, &mvpMatrix);
    pDrawParam->modulateType = 3;
}

void FFLiInvalidateDrawParamRawMaskParts(FFLiRawMaskPartsDrawParam* pDrawParam)
{
    InvalidatePrimitive(&pDrawParam->primitiveParam);
    InvalidateAttributes(&pDrawParam->attributeBufferParam);
}

void FFLiInitDrawParamRawMaskPartsFill(FFLiRawMaskPartsDrawParam* pDrawParam, FFLiBufferAllocator* pAllocator)
{
    InitPrimitive(&pDrawParam->primitiveParam, pAllocator);
    InitAttributesForFill(&pDrawParam->attributeBufferParam, pAllocator);
    pDrawParam->modulateType = 3;
}

void FFLiDrawRawMaskParts(const FFLiRawMaskPartsDrawParam* pDrawParam, const FFLiShaderCallback* pCallback)
{
    pCallback->CallDraw(*pDrawParam);
}

namespace {

void CalcMVMatrix(Mat34* pMVMatrix, const FFLiRawMaskPartsDesc* pDesc)
{
    Mat34 m;

    MAT34Scale(pMVMatrix, pDesc->scale.x, pDesc->scale.y, 1.0f);

    MAT34RotRad(&m, 'z', pDesc->rot * (3.14159265358979323846f / 180.0f));
    MAT34Concat(&m, pMVMatrix, pMVMatrix);

    MAT34Scale(&m, 0.88961464f, 0.9276675f, 1.0f);
    MAT34Concat(&m, pMVMatrix, pMVMatrix);

    MAT34Trans(&m, pDesc->pos.x, pDesc->pos.y, 0.0f);
    MAT34Concat(&m, pMVMatrix, pMVMatrix);
}

void* Allocate(FFLiBufferAllocator* pAllocator, u32 size, u32 alignment)
{
    return FFLiAllocateBufferAllocator(pAllocator, size, alignment);
}

void EndianSwap(void* ptr, u32 size)
{
    FFLiBugEndianSwap(ptr, size);
}

void InitPrimitive(FFLPrimitiveParam* pPrimitive, FFLiBufferAllocator* pAllocator)
{
    const u32 INDEX_BUFFER_SIZE = sizeof(u16) * 4;

    static const u16 INDEX_BUFFER[4] = { 2, 1, 3, 0 };
    NN_STATIC_ASSERT(sizeof(INDEX_BUFFER) == INDEX_BUFFER_SIZE);

    pPrimitive->primitiveType = GX2_PRIMITIVE_TRIANGLE_STRIP;
    pPrimitive->indexCount = 4;
    pPrimitive->indexFormat = GX2_INDEX_FORMAT_U16;
    pPrimitive->pIndexBuffer = FFLiBugVgtFixedIndexPtr(Allocate(pAllocator, FFLiBugCanVgtFixedIndexSize(FFLiBugCanSwapSize(INDEX_BUFFER_SIZE)), GX2_INDEX_BUFFER_ALIGNMENT));

    std::memcpy(pPrimitive->pIndexBuffer, INDEX_BUFFER, INDEX_BUFFER_SIZE);
    EndianSwap(pPrimitive->pIndexBuffer, INDEX_BUFFER_SIZE);
}

void CalcAttribute(FFLVec4* pPosBuf, FFLVec2* pTexBuf, FFLiOriginPosition originPosition, const Mat44* pMVPMatrix)
{
    f32 posXAdd = 0.0f;
    f32 texCoordX01 = 0.0f;
    f32 texCoordX23 = 0.0f;

    switch (originPosition)
    {
    case FFLI_ORIGIN_POSITION_CENTER:
        posXAdd = -0.5f;
        texCoordX01 = 1.0f;
        break;
    case FFLI_ORIGIN_POSITION_RIGHT:
        texCoordX23 = 1.0f;
        break;
    case FFLI_ORIGIN_POSITION_LEFT:
        posXAdd = -1.0f;
        texCoordX01 = 1.0f;
        break;
    }

    f32 posX[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
    f32 posY[4] = { -0.5f, 0.5f, 0.5f, -0.5f };
    f32 texCoordY[4] = { 0.0f, 1.0f, 1.0f, 0.0f };

    for (u32 i = 0; i < 4; i++)
    {
        pPosBuf[i].x = posX[i] + posXAdd;
        pPosBuf[i].y = posY[i];
        pPosBuf[i].z = 0.0f;
        pPosBuf[i].w = 0.0f;

        pTexBuf[i].y = texCoordY[i];
    }

    pTexBuf[0].x = texCoordX01;
    pTexBuf[1].x = texCoordX01;
    pTexBuf[2].x = texCoordX23;
    pTexBuf[3].x = texCoordX23;

    for (u32 i = 0; i < 4; i++)
        MAT44MultVec(pMVPMatrix, (VecPtr)&(pPosBuf[i]), (VecPtr)&(pPosBuf[i]));
}

void InitAttributes(FFLAttributeBufferParam* pAttributes, FFLiOriginPosition originPosition, FFLiBufferAllocator* pAllocator, const Mat44* pMVPMatrix)
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec4) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    static const FFLAttributeBufferParam ATTRIBUTES = { {
        { POSITION_BUFFER_SIZE, sizeof(FFLVec4) },
        { TEXCOORD_BUFFER_SIZE, sizeof(FFLVec2) },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 }
    } };

    std::memcpy(pAttributes, &ATTRIBUTES, sizeof(FFLAttributeBufferParam));

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr = Allocate(pAllocator, FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), GX2_VERTEX_BUFFER_ALIGNMENT);
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr = Allocate(pAllocator, FFLiBugCanSwapSize(TEXCOORD_BUFFER_SIZE), GX2_VERTEX_BUFFER_ALIGNMENT);

    CalcAttribute(
        static_cast<FFLVec4*>(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr),
        static_cast<FFLVec2*>(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr),
        originPosition,
        pMVPMatrix
    );

    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER_SIZE);
    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr, TEXCOORD_BUFFER_SIZE);
}

void InvalidatePrimitive(FFLPrimitiveParam* pPrimitive)
{
    FFLiInvalidate(
        GX2_INVALIDATE_CPU_ATTRIB_BUFFER,
        pPrimitive->pIndexBuffer,
        sizeof(u16) * pPrimitive->indexCount    // Apparently Nintendo forgot the index count is 4
    );
}

void InvalidateAttributes(FFLAttributeBufferParam* pAttributes)
{
    for (u32 i = 0; i < FFL_ATTRIBUTE_BUFFER_TYPE_MAX; i++)
    {
        void* ptr = pAttributes->attributeBuffers[i].ptr;
        if (ptr != NULL)
            FFLiInvalidate(
                GX2_INVALIDATE_CPU_ATTRIB_BUFFER,
                ptr,
                pAttributes->attributeBuffers[i].size
            );
    }
}

void InitAttributesForFill(FFLAttributeBufferParam* pAttributes, FFLiBufferAllocator* pAllocator)
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec4) * 4;

    static const FFLVec4 POSITION_BUFFER[4] = {
        {  1.0f, -1.0f,  0.0f,  0.0f },
        {  1.0f,  1.0f,  0.0f,  0.0f },
        { -1.0f,  1.0f,  0.0f,  0.0f },
        { -1.0f, -1.0f,  0.0f,  0.0f }
    };
    NN_STATIC_ASSERT(sizeof(POSITION_BUFFER) == POSITION_BUFFER_SIZE);

    static const FFLAttributeBufferParam ATTRIBUTES = { {
        { POSITION_BUFFER_SIZE, sizeof(FFLVec4) },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 }
    } };

    std::memcpy(pAttributes, &ATTRIBUTES, sizeof(FFLAttributeBufferParam));

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr = Allocate(pAllocator, FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), GX2_VERTEX_BUFFER_ALIGNMENT);

    std::memcpy(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER, POSITION_BUFFER_SIZE);
    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER_SIZE);
}

}
