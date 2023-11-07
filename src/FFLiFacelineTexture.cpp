#include <nn/ffl/FFLColor.h>
#include <nn/ffl/FFLDrawParam.h>
#include <nn/ffl/FFLVec.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiFacelineTexture.h>
#include <nn/ffl/FFLiFacelineTextureTempObject.h>
#include <nn/ffl/FFLiManager.h>
#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiModulate.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiResourceLoader.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBug.h>
#include <nn/ffl/detail/FFLiCharInfo.h>
#include <nn/ffl/detail/FFLiCopySurface.h>

#include <gpu/rio_RenderState.h>
#include <misc/rio_MemUtil.h>

#if RIO_IS_CAFE
#include <gx2/mem.h>
#endif // RIO_IS_CAFE

#include <cstring>

namespace {

u32 GetWidth(u32 resolution);
u32 GetHeight(u32 resolution);
u32 GetNumMips(u32 width, u32 height, bool enableMipMap);

rio::TextureFormat GetTextureFormat(bool useOffScreenSrgbFetch);

void DeleteTexture_FaceLine(FFLiFacelineTextureTempObject* pObject, bool isExpand);
void DeleteTexture_FaceMake(FFLiFacelineTextureTempObject* pObject, bool isExpand);
void DeleteTexture_FaceBeard(FFLiFacelineTextureTempObject* pObject, const FFLiCharInfo* pCharInfo, bool isExpand);

void* Allocate(u32 size, u32 alignment);
void Free(void* ptr);

void EndianSwap(void* ptr, u32 size);

void InitPrimitive(FFLPrimitiveParam* pPrimitive);
void InitAttributes(FFLAttributeBufferParam* pAttributes, u32 resolution);
void InitDrawParamWithoutModulate(FFLDrawParam* pDrawParam, u32 resolution);

void DeletePrimitive(FFLPrimitiveParam* pPrimitive);
void DeleteAttributes(FFLAttributeBufferParam* pAttributes);
void DeleteDrawParam(FFLDrawParam* pDrawParam);

void InvalidatePrimitive(FFLPrimitiveParam* pPrimitive);
void InvalidateAttributes(FFLAttributeBufferParam* pAttributes);
void InvalidateDrawParam(FFLDrawParam* pDrawParam);

}

void FFLiInitFacelineTexture(FFLiRenderTexture* pRenderTexture, u32 resolution, bool enableMipMap)
{
    u32 width = GetWidth(resolution);
    u32 height = GetHeight(resolution);
    u32 numMips = GetNumMips(width, height, enableMipMap);
    rio::TextureFormat format = GetTextureFormat(FFLiUseOffScreenSrgbFetch());
    FFLiInitRenderTexture(pRenderTexture, width, height, format, numMips);
}

void FFLiDeleteFacelineTexture(FFLiRenderTexture* pRenderTexture)
{
    FFLiDeleteRenderTexture(pRenderTexture);
}

FFLResult FFLiInitTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiRenderTextureBuffer* pRenderTextureBuffer)
{
    std::memset(pObject, 0, sizeof(FFLiFacelineTextureTempObject));

    FFLResult result = FFLiLoadTextureWithAllocate(&pObject->pTextureFaceLine, FFLI_TEXTURE_PARTS_TYPE_FACELINE, pCharInfo->parts.faceLine, pResLoader);
    if (result != FFL_RESULT_OK)
        return result;

    result = FFLiLoadTextureWithAllocate(&pObject->pTextureFaceMake, FFLI_TEXTURE_PARTS_TYPE_FACE_MAKEUP, pCharInfo->parts.faceMakeup, pResLoader);
    if (result != FFL_RESULT_OK)
    {
        DeleteTexture_FaceLine(pObject, pResLoader->IsExpand());
        return result;
    }

    s32 beardType = pCharInfo->parts.beardType;
    bool enableBeardTexture = beardType >= 4;

    if (enableBeardTexture)
    {
        result = FFLiLoadTextureWithAllocate(&pObject->pTextureFaceBeard, FFLI_TEXTURE_PARTS_TYPE_BEARD, beardType - 3, pResLoader);
        if (result != FFL_RESULT_OK)
        {
            DeleteTexture_FaceMake(pObject, pResLoader->IsExpand());
            DeleteTexture_FaceLine(pObject, pResLoader->IsExpand());
            return result;
        }
    }

    InitDrawParamWithoutModulate(&pObject->drawParamFaceLine, resolution);
    InitDrawParamWithoutModulate(&pObject->drawParamFaceMake, resolution);
    InitDrawParamWithoutModulate(&pObject->drawParamFaceBeard, resolution);

    FFLiInitModulateFaceLine(&pObject->drawParamFaceLine.modulateParam, *pObject->pTextureFaceLine);
    FFLiInitModulateFaceMake(&pObject->drawParamFaceMake.modulateParam, *pObject->pTextureFaceMake);
    if (enableBeardTexture)
        FFLiInitModulateFaceBeard(&pObject->drawParamFaceBeard.modulateParam, pCharInfo->parts.beardColor, *pObject->pTextureFaceBeard);

    return FFL_RESULT_OK;
}

void FFLiDeleteTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, const FFLiCharInfo* pCharInfo, FFLResourceType resourceType)
{
    DeleteDrawParam(&pObject->drawParamFaceBeard);
    DeleteDrawParam(&pObject->drawParamFaceMake);
    DeleteDrawParam(&pObject->drawParamFaceLine);

    RIO_ASSERT(FFLiManager::IsConstruct());
    bool isExpand = FFLiManager::GetInstance()->GetResourceManager().IsExpand(resourceType);

    DeleteTexture_FaceBeard(pObject, pCharInfo, isExpand);
    DeleteTexture_FaceMake(pObject, isExpand);
    DeleteTexture_FaceLine(pObject, isExpand);
}

void FFLiRenderFacelineTexture(FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, FFLiFacelineTextureTempObject* pObject, const FFLiShaderCallback* pCallback
#if RIO_IS_CAFE
    , FFLiCopySurface* pCopySurface
#endif // RIO_IS_CAFE
)
{
    bool enableBeardTexture = pCharInfo->parts.beardType >= 4;

    InvalidateDrawParam(&pObject->drawParamFaceLine);
    InvalidateDrawParam(&pObject->drawParamFaceMake);
    InvalidateDrawParam(&pObject->drawParamFaceBeard);

    pObject->pTextureFaceLine->invalidateGPUCache();
    pObject->pTextureFaceMake->invalidateGPUCache();
    if (enableBeardTexture)
        pObject->pTextureFaceBeard->invalidateGPUCache();

    FFLColor facelineColor = FFLiGetSrgbFetchFacelineColor(pCharInfo->parts.facelineColor);

    rio::RenderState renderState;
    renderState.setBlendEnable(true);
    renderState.setDepthEnable(false, false);
    renderState.setCullingMode(rio::Graphics::CULLING_MODE_NONE);
    renderState.setBlendFactorSeparate(
        rio::Graphics::BLEND_MODE_SRC_ALPHA, rio::Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA,
        rio::Graphics::BLEND_MODE_ONE, rio::Graphics::BLEND_MODE_ONE
    );
    renderState.setBlendEquationSeparate(
        rio::Graphics::BLEND_FUNC_ADD,
        rio::Graphics::BLEND_FUNC_MAX
    );
    renderState.apply();

    pCallback->CallApplyAlphaTestDisable();

    FFLiRenderTexture& renderTexture = *pRenderTexture;

    FFLiInvalidateRenderTexture(&renderTexture);
    RIO_ASSERT(renderTexture.pTextureData->getTextureFormat() == agl::cTextureFormat_R8_G8_B8_A8_uNorm);
    FFLiSetupRenderTexture(&renderTexture, &facelineColor, NULL, 0, pCallback);

    pCallback->CallDraw(pObject->drawParamFaceMake);
    pCallback->CallDraw(pObject->drawParamFaceLine);
    if (pObject->pTextureFaceBeard != NULL)
        pCallback->CallDraw(pObject->drawParamFaceBeard);

    if (renderTexture.pTextureData->getMipLevelNum() > 1)
    {
#if RIO_IS_WIN
        renderTexture.pTextureData->getHandle()->bind();
        RIO_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
#elif RIO_IS_CAFE
        pCopySurface->Begin();

        GX2Surface* pSurface = const_cast<GX2Surface*>(&renderTexture.pTextureData->getSurface());

        for (u32 i = 1; i < renderTexture.pTextureData->getMipLevelNum(); i++)
            pCopySurface->Execute(pSurface, i, pSurface, i - 1);

        pCopySurface->End();
#endif
    }

    FFLiFlushRenderTexture(&renderTexture);
}

namespace {

u32 GetWidth(u32 resolution)
{
    return resolution / 2;
}

u32 GetHeight(u32 resolution)
{
    return resolution;
}

u32 GetNumMips(u32 width, u32 height, bool enableMipMap)
{
    u32 ret = 0;

    if (enableMipMap)
        ret = FFLiGetMipMapNum(width, height);

    else
        ret = 1;

    return ret;
}

rio::TextureFormat GetTextureFormat(bool useOffScreenSrgbFetch)
{
    if (useOffScreenSrgbFetch)
        return rio::TEXTURE_FORMAT_R8_G8_B8_A8_SRGB;

    else
        return rio::TEXTURE_FORMAT_R8_G8_B8_A8_UNORM;
}

void DeleteTexture_FaceLine(FFLiFacelineTextureTempObject* pObject, bool isExpand)
{
    FFLiDeleteTexture(&pObject->pTextureFaceLine, isExpand);
}

void DeleteTexture_FaceMake(FFLiFacelineTextureTempObject* pObject, bool isExpand)
{
    FFLiDeleteTexture(&pObject->pTextureFaceMake, isExpand);
}

void DeleteTexture_FaceBeard(FFLiFacelineTextureTempObject* pObject, const FFLiCharInfo* pCharInfo, bool isExpand)
{
    s32 beardType = pCharInfo->parts.beardType;
    bool enableBeardTexture = beardType >= 4;

    if (enableBeardTexture)
        FFLiDeleteTexture(&pObject->pTextureFaceBeard, isExpand);
}

void* Allocate(u32 size, u32 alignment)
{
    return rio::MemUtil::alloc(size, alignment);
}

void Free(void* ptr)
{
    rio::MemUtil::free(ptr);
}

void EndianSwap(void* ptr, u32 size)
{
    FFLiBugEndianSwap(ptr, size);
}

void InitPrimitive(FFLPrimitiveParam* pPrimitive)
{
    const u32 INDEX_BUFFER_SIZE = sizeof(u16) * 4;

    static const u16 INDEX_BUFFER[4] = { 0, 1, 2, 3 };
    NN_STATIC_ASSERT(sizeof(INDEX_BUFFER) == INDEX_BUFFER_SIZE);

    pPrimitive->primitiveType = rio::Drawer::TRIANGLE_STRIP;
    pPrimitive->indexCount = 4;
    pPrimitive->pIndexBuffer = FFLiBugVgtFixedIndexPtr(Allocate(FFLiBugCanSwapSize(FFLiBugCanVgtFixedIndexSize(INDEX_BUFFER_SIZE)), rio::Drawer::cIdxAlignment));

    std::memcpy(pPrimitive->pIndexBuffer, INDEX_BUFFER, INDEX_BUFFER_SIZE);
    EndianSwap(pPrimitive->pIndexBuffer, INDEX_BUFFER_SIZE);
}

void InitAttributes(FFLAttributeBufferParam* pAttributes, u32 resolution)
{
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec4) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    const FFLVec4 POSITION_BUFFER[4] = {
        { -1.0f,  1.0f,  0.0f,  0.0f },
        {  1.0f,  1.0f,  0.0f,  0.0f },
        { -1.0f, -1.0f,  0.0f,  0.0f },
        {  1.0f, -1.0f,  0.0f,  0.0f }
    };
    NN_STATIC_ASSERT(sizeof(POSITION_BUFFER) == POSITION_BUFFER_SIZE);

    static const FFLVec2 TEXCOORD_BUFFER[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f }
    };
    NN_STATIC_ASSERT(sizeof(TEXCOORD_BUFFER) == TEXCOORD_BUFFER_SIZE);

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].size = POSITION_BUFFER_SIZE;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].stride = sizeof(FFLVec4);
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr = Allocate(FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), rio::Drawer::cVtxAlignment);

    std::memcpy(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER, POSITION_BUFFER_SIZE);
    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER_SIZE);

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].size = TEXCOORD_BUFFER_SIZE;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].stride = sizeof(FFLVec2);
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr = Allocate(FFLiBugCanSwapSize(TEXCOORD_BUFFER_SIZE), rio::Drawer::cVtxAlignment);

    std::memcpy(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr, TEXCOORD_BUFFER, TEXCOORD_BUFFER_SIZE);
    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr, TEXCOORD_BUFFER_SIZE);

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL].size = 0;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL].stride = 0;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL].ptr = NULL;

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT].size = 0;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT].stride = 0;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT].ptr = NULL;

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_COLOR].size = 0;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_COLOR].stride = 0;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_COLOR].ptr = NULL;
}

void InitDrawParamWithoutModulate(FFLDrawParam* pDrawParam, u32 resolution)
{
    pDrawParam->cullMode = FFL_CULL_MODE_MAX;
    InitPrimitive(&pDrawParam->primitiveParam);
    InitAttributes(&pDrawParam->attributeBufferParam, resolution);
}

void DeletePrimitive(FFLPrimitiveParam* pPrimitive)
{
    Free(FFLiBugVgtFixedIndexOriginalPtr(pPrimitive->pIndexBuffer));
}

void DeleteAttributes(FFLAttributeBufferParam* pAttributes)
{
    Free(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr);
    Free(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr);
}

void DeleteDrawParam(FFLDrawParam* pDrawParam)
{
    DeletePrimitive(&pDrawParam->primitiveParam);
    DeleteAttributes(&pDrawParam->attributeBufferParam);
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

void InvalidateDrawParam(FFLDrawParam* pDrawParam)
{
    InvalidatePrimitive(&pDrawParam->primitiveParam);
    InvalidateAttributes(&pDrawParam->attributeBufferParam);
}

}
