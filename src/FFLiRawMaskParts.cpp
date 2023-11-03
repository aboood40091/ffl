#include <nn/ffl/FFLiRawMaskParts.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBug.h>

#include <math/rio_Matrix.h>
#include <misc/rio_MemUtil.h>

#if RIO_IS_CAFE
#include <gx2/mem.h>
#endif // RIO_IS_CAFE

#include <cstring>

namespace {

void CalcMVMatrix(rio::Matrix34f* pMVMatrix, const FFLiRawMaskPartsDesc* pDesc);

void InitPrimitive(FFLPrimitiveParam* pPrimitive);
void InitAttributes(FFLAttributeBufferParam* pAttributes, FFLiOriginPosition originPosition, const rio::BaseMtx44f* pMVPMatrix);

void InvalidatePrimitive(FFLPrimitiveParam* pPrimitive);
void InvalidateAttributes(FFLAttributeBufferParam* pAttributes);

void InitAttributesForFill(FFLAttributeBufferParam* pAttributes);

}

void FFLiInitDrawParamRawMaskParts(FFLiRawMaskPartsDrawParam* pDrawParam, const FFLiRawMaskPartsDesc* pDesc, const rio::BaseMtx44f* pProjMatrix)
{
    rio::Matrix44f mvpMatrix;
    rio::Matrix34f mvMatrix;

    CalcMVMatrix(&mvMatrix, pDesc);
    mvpMatrix.fromMatrix34(mvMatrix);
    mvpMatrix.setMul(static_cast<const rio::Matrix44f&>(*pProjMatrix), mvpMatrix);

    InitPrimitive(&pDrawParam->primitiveParam);
    InitAttributes(&pDrawParam->attributeBufferParam, pDesc->originPos, &mvpMatrix);
    pDrawParam->cullMode = FFL_CULL_MODE_MAX;
}

void FFLiInvalidateDrawParamRawMaskParts(FFLiRawMaskPartsDrawParam* pDrawParam)
{
    InvalidatePrimitive(&pDrawParam->primitiveParam);
    InvalidateAttributes(&pDrawParam->attributeBufferParam);
}

void FFLiInitDrawParamRawMaskPartsFill(FFLiRawMaskPartsDrawParam* pDrawParam)
{
    InitPrimitive(&pDrawParam->primitiveParam);
    InitAttributesForFill(&pDrawParam->attributeBufferParam);
    pDrawParam->cullMode = FFL_CULL_MODE_MAX;
}

void FFLiDrawRawMaskParts(const FFLiRawMaskPartsDrawParam* pDrawParam, const FFLiShaderCallback* pCallback)
{
    pCallback->CallDraw(*pDrawParam);
}

namespace {

void CalcMVMatrix(rio::Matrix34f* pMVMatrix, const FFLiRawMaskPartsDesc* pDesc)
{
    pMVMatrix->makeSRT(
        { pDesc->scale.x * 0.88961464f, pDesc->scale.y * 0.9276675f, 1.0f },
        { 0.0f, 0.0f, rio::Mathf::deg2rad(pDesc->rot) },
        { pDesc->pos.x, pDesc->pos.y, 0.0f }
    );
}

void* Allocate(u32 size, u32 alignment)
{
    return rio::MemUtil::alloc(size, alignment);
}

void EndianSwap(void* ptr, u32 size)
{
    FFLiBugEndianSwap(ptr, size);
}

void InitPrimitive(FFLPrimitiveParam* pPrimitive)
{
    const u32 INDEX_BUFFER_SIZE = sizeof(u16) * 4;

    static const u16 INDEX_BUFFER[4] = { 2, 1, 3, 0 };
    NN_STATIC_ASSERT(sizeof(INDEX_BUFFER) == INDEX_BUFFER_SIZE);

    pPrimitive->primitiveType = rio::Drawer::TRIANGLE_STRIP;
    pPrimitive->indexCount = 4;
    pPrimitive->pIndexBuffer = FFLiBugVgtFixedIndexPtr(Allocate(FFLiBugCanVgtFixedIndexSize(FFLiBugCanSwapSize(INDEX_BUFFER_SIZE)), rio::Drawer::cIdxAlignment));

    std::memcpy(pPrimitive->pIndexBuffer, INDEX_BUFFER, INDEX_BUFFER_SIZE);
    EndianSwap(pPrimitive->pIndexBuffer, INDEX_BUFFER_SIZE);
}

void CalcAttribute(FFLVec4* pPosBuf, FFLVec2* pTexBuf, FFLiOriginPosition originPosition, const rio::BaseMtx44f* pMVPMatrix)
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
    {
        const f32 w = pMVPMatrix->m[3][0] * pPosBuf[i].x + pMVPMatrix->m[3][1] * pPosBuf[i].y + pMVPMatrix->m[3][2] * pPosBuf[i].z + pMVPMatrix->m[3][3];
        const f32 w_inv = 1 / w;

        pPosBuf[i] = FFLVec4 {
            (pMVPMatrix->m[0][0] * pPosBuf[i].x + pMVPMatrix->m[0][1] * pPosBuf[i].y + pMVPMatrix->m[0][2] * pPosBuf[i].z + pMVPMatrix->m[0][3]) * w_inv,
            (pMVPMatrix->m[1][0] * pPosBuf[i].x + pMVPMatrix->m[1][1] * pPosBuf[i].y + pMVPMatrix->m[1][2] * pPosBuf[i].z + pMVPMatrix->m[1][3]) * w_inv,
            (pMVPMatrix->m[2][0] * pPosBuf[i].x + pMVPMatrix->m[2][1] * pPosBuf[i].y + pMVPMatrix->m[2][2] * pPosBuf[i].z + pMVPMatrix->m[2][3]) * w_inv,
            pPosBuf[i].w
        };
    }
}

void InitAttributes(FFLAttributeBufferParam* pAttributes, FFLiOriginPosition originPosition, const rio::BaseMtx44f* pMVPMatrix)
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

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr = Allocate(FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), rio::Drawer::cVtxAlignment);
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr = Allocate(FFLiBugCanSwapSize(TEXCOORD_BUFFER_SIZE), rio::Drawer::cVtxAlignment);

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
#if RIO_IS_CAFE
    GX2Invalidate(
        GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
        pPrimitive->pIndexBuffer,
        sizeof(u16) * pPrimitive->indexCount    // Apparently Nintendo forgot the index count is 4
    );
#endif // RIO_IS_CAFE
}

void InvalidateAttributes(FFLAttributeBufferParam* pAttributes)
{
#if RIO_IS_CAFE
    for (u32 i = 0; i < FFL_ATTRIBUTE_BUFFER_TYPE_MAX; i++)
    {
        void* ptr = pAttributes->attributeBuffers[i].ptr;
        if (ptr != NULL)
            GX2Invalidate(
                GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER,
                ptr,
                pAttributes->attributeBuffers[i].size
            );
    }
#endif // RIO_IS_CAFE
}

void InitAttributesForFill(FFLAttributeBufferParam* pAttributes)
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

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr = Allocate(FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), rio::Drawer::cVtxAlignment);

    std::memcpy(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER, POSITION_BUFFER_SIZE);
    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER_SIZE);
}

}
