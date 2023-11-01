#include <nn/ffl/FFLColor.h>
#include <nn/ffl/FFLDrawParam.h>

#include <nn/ffl/FFLiColor.h>
#include <nn/ffl/FFLiFacelineTexture.h>
#include <nn/ffl/FFLiFacelineTextureTempObject.h>
#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiModulate.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiBug.h>
#include <nn/ffl/detail/FFLiCharInfo.h>
#include <nn/ffl/detail/FFLiCopySurface.h>

#include <gpu/rio_RenderState.h>

#include <cstring>

namespace {

u32 GetWidth(u32 resolution);
u32 GetHeight(u32 resolution);
u32 GetNumMips(u32 width, u32 height, bool enableMipMap);

u32 GetTextureBufferSize(u32 width, u32 height, u32 numMips);
u32 GetDrawParamBufferSize();

rio::TextureFormat GetTextureFormat(bool useOffScreenSrgbFetch);

void* Allocate(FFLiBufferAllocator* pAllocator, u32 size, u32 alignment);

void EndianSwap(void* ptr, u32 size);

void InitPrimitive(FFLPrimitiveParam* pPrimitive, FFLiBufferAllocator* pAllocator);
void InitAttributes(FFLAttributeBufferParam* pAttributes, u32 resolution, FFLiBufferAllocator* pAllocator);
void InitDrawParamWithoutModulate(FFLDrawParam* pDrawParam, u32 resolution, FFLiBufferAllocator* pAllocator);

void InvalidatePrimitive(FFLPrimitiveParam* pPrimitive);
void InvalidateAttributes(FFLAttributeBufferParam* pAttributes);
void InvalidateDrawParam(FFLDrawParam* pDrawParam);

}

u32 FFLiGetBufferSizeFacelineTexture(u32 resolution, bool enableMipMap)
{
    u32 width = GetWidth(resolution);
    u32 height = GetHeight(resolution);
    u32 numMips = GetNumMips(width, height, enableMipMap);
    return GetTextureBufferSize(width, height, numMips);
}

u32 FFLiGetTempBufferSizeFacelineTexture(u32 resolution, bool enableMipMap, FFLiResourceManager* pResourceManager, FFLResourceType resourceType)
{
    u32 ret  = FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_FACELINE);
    ret     += FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_FACE_MAKEUP);
    ret     += FFLiGetTextureMaxSizeWithAlign(pResourceManager, resourceType, FFLI_TEXTURE_PARTS_TYPE_BEARD);
    ret     += GetDrawParamBufferSize() * 3;

    return ret;
}

void FFLiInitFacelineTexture(FFLiRenderTexture* pRenderTexture, u32 resolution, bool enableMipMap, FFLiBufferAllocator* pAllocator)
{
    u32 width = GetWidth(resolution);
    u32 height = GetHeight(resolution);
    u32 numMips = GetNumMips(width, height, enableMipMap);
    rio::TextureFormat format = GetTextureFormat(FFLiUseOffScreenSrgbFetch());
    FFLiInitRenderTexture(pRenderTexture, width, height, format, numMips, pAllocator);
}

FFLResult FFLiInitTempObjectFacelineTexture(FFLiFacelineTextureTempObject* pObject, FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, bool enableMipMap, FFLiResourceLoader* pResLoader, FFLiBufferAllocator* pAllocator, FFLiRenderTextureBuffer* pRenderTextureBuffer)
{
    std::memset(pObject, 0, sizeof(FFLiFacelineTextureTempObject));

    FFLResult result = FFLiLoadTextureWithAllocate(&pObject->pTextureFaceLine, FFLI_TEXTURE_PARTS_TYPE_FACELINE, pCharInfo->parts.faceLine, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    result = FFLiLoadTextureWithAllocate(&pObject->pTextureFaceMake, FFLI_TEXTURE_PARTS_TYPE_FACE_MAKEUP, pCharInfo->parts.faceMakeup, pResLoader, pAllocator);
    if (result != FFL_RESULT_OK)
        return result;

    s32 beardType = pCharInfo->parts.beardType;
    bool enableBeardTexture = beardType >= 4;

    if (enableBeardTexture)
    {
        result = FFLiLoadTextureWithAllocate(&pObject->pTextureFaceBeard, FFLI_TEXTURE_PARTS_TYPE_BEARD, beardType - 3, pResLoader, pAllocator);
        if (result != FFL_RESULT_OK)
            return result;
    }

    InitDrawParamWithoutModulate(&pObject->drawParamFaceLine, resolution, pAllocator);
    InitDrawParamWithoutModulate(&pObject->drawParamFaceMake, resolution, pAllocator);
    InitDrawParamWithoutModulate(&pObject->drawParamFaceBeard, resolution, pAllocator);

    FFLiInitModulateFaceLine(&pObject->drawParamFaceLine.modulateParam, *pObject->pTextureFaceLine);
    FFLiInitModulateFaceMake(&pObject->drawParamFaceMake.modulateParam, *pObject->pTextureFaceMake);
    if (enableBeardTexture)
        FFLiInitModulateFaceBeard(&pObject->drawParamFaceBeard.modulateParam, pCharInfo->parts.beardColor, *pObject->pTextureFaceBeard);

    return FFL_RESULT_OK;
}

void FFLiRenderFacelineTexture(FFLiRenderTexture* pRenderTexture, const FFLiCharInfo* pCharInfo, u32 resolution, FFLiFacelineTextureTempObject* pObject, const FFLiShaderCallback* pCallback, FFLiCopySurface* pCopySurface)
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
    renderState.setCullingMode(rio::Graphics::CULLING_MODE_BACK);
    renderState.setBlendFactorSeparate(
        rio::Graphics::BLEND_MODE_SRC_ALPHA, rio::Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA,
        rio::Graphics::BLEND_MODE_ONE, rio::Graphics::BLEND_MODE_ONE
    );
    renderState.setBlendEquationSeparate(
        rio::Graphics::BLEND_FUNC_ADD,
        rio::Graphics::BLEND_FUNC_MAX
    );
    renderState.apply();

    FFLiRenderTexture& renderTexture = *pRenderTexture;

    FFLiInvalidateRenderTexture(&renderTexture);
    RIO_ASSERT(renderTexture.textureData.getTextureFormat() == agl::cTextureFormat_R8_G8_B8_A8_uNorm);
    FFLiSetupRenderTexture(&renderTexture, &facelineColor, NULL, 0, pCallback);

    pCallback->CallDraw(pObject->drawParamFaceMake);
    pCallback->CallDraw(pObject->drawParamFaceLine);
    if (pObject->pTextureFaceBeard != NULL)
        pCallback->CallDraw(pObject->drawParamFaceBeard);

    if (renderTexture.textureData.getMipLevelNum() > 1)
    {
        pCopySurface->Begin();
        for (u32 i = 1; i < renderTexture.textureData.getMipLevelNum(); i++)
            pCopySurface->Execute(&renderTexture.textureData, i, i - 1);
    }

    pCallback->CallSetContextState();

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

u32 GetTextureBufferSize(u32 width, u32 height, u32 numMips)
{
    return FFLiGetBufferRenderTexture(width, height, rio::TEXTURE_FORMAT_R8_G8_B8_A8_UNORM, numMips);
}

u32 GetDrawParamBufferSize()
{
    const u32 INDEX_BUFFER_SIZE = sizeof(u16) * 4;
    const u32 POSITION_BUFFER_SIZE = sizeof(FFLVec4) * 4;
    const u32 TEXCOORD_BUFFER_SIZE = sizeof(FFLVec2) * 4;

    u32 ret  = rio::Drawer::cVtxAlignment; // max(rio::Drawer::cIdxAlignment, rio::Drawer::cVtxAlignment)
    ret     += FFLiRoundUp(FFLiBugCanVgtFixedIndexSize(FFLiBugCanSwapSize(INDEX_BUFFER_SIZE)), rio::Drawer::cIdxAlignment);
    ret     += FFLiRoundUp(FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), rio::Drawer::cVtxAlignment);
    ret     += FFLiRoundUp(FFLiBugCanSwapSize(TEXCOORD_BUFFER_SIZE), rio::Drawer::cVtxAlignment);
    return ret;
}

rio::TextureFormat GetTextureFormat(bool useOffScreenSrgbFetch)
{
    if (useOffScreenSrgbFetch)
        return rio::TEXTURE_FORMAT_R8_G8_B8_A8_SRGB;

    else
        return rio::TEXTURE_FORMAT_R8_G8_B8_A8_UNORM;
}

void* Allocate(FFLiBufferAllocator* pAllocator, u32 size, u32 alignment)
{
    return pAllocator->Allocate(size, alignment);
}

void EndianSwap(void* ptr, u32 size)
{
    FFLiBugEndianSwap(ptr, size);
}

void InitPrimitive(FFLPrimitiveParam* pPrimitive, FFLiBufferAllocator* pAllocator)
{
    const u32 INDEX_BUFFER_SIZE = sizeof(u16) * 4;

    static const u16 INDEX_BUFFER[4] = { 0, 1, 2, 3 };
    NN_STATIC_ASSERT(sizeof(INDEX_BUFFER) == INDEX_BUFFER_SIZE);

    pPrimitive->primitiveType = rio::Drawer::TRIANGLE_STRIP;
    pPrimitive->indexCount = 4;
    pPrimitive->pIndexBuffer = FFLiBugVgtFixedIndexPtr(Allocate(pAllocator, FFLiBugCanSwapSize(FFLiBugCanVgtFixedIndexSize(INDEX_BUFFER_SIZE)), rio::Drawer::cIdxAlignment));

    std::memcpy(pPrimitive->pIndexBuffer, INDEX_BUFFER, INDEX_BUFFER_SIZE);
    EndianSwap(pPrimitive->pIndexBuffer, INDEX_BUFFER_SIZE);
}

void InitAttributes(FFLAttributeBufferParam* pAttributes, u32 resolution, FFLiBufferAllocator* pAllocator)
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
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr = Allocate(pAllocator, FFLiBugCanSwapSize(POSITION_BUFFER_SIZE), rio::Drawer::cVtxAlignment);

    std::memcpy(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER, POSITION_BUFFER_SIZE);
    EndianSwap(pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_POSITION].ptr, POSITION_BUFFER_SIZE);

    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].size = TEXCOORD_BUFFER_SIZE;
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].stride = sizeof(FFLVec2);
    pAttributes->attributeBuffers[FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD].ptr = Allocate(pAllocator, FFLiBugCanSwapSize(TEXCOORD_BUFFER_SIZE), rio::Drawer::cVtxAlignment);

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

void InitDrawParamWithoutModulate(FFLDrawParam* pDrawParam, u32 resolution, FFLiBufferAllocator* pAllocator)
{
    pDrawParam->cullMode = FFL_CULL_MODE_MAX;
    InitPrimitive(&pDrawParam->primitiveParam, pAllocator);
    InitAttributes(&pDrawParam->attributeBufferParam, resolution, pAllocator);
}

void InvalidatePrimitive(FFLPrimitiveParam* pPrimitive)
{
#if RIO_IS_CAFE
    GX2Invalidate(
        GX2_INVALIDATE_CPU_ATTRIB_BUFFER,
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
                GX2_INVALIDATE_CPU_ATTRIB_BUFFER,
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
