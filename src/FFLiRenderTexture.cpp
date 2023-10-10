#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiRenderTextureBuffer.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

#include <cstring>

u32 FFLiGetBufferRenderTexture(u32 width, u32 height, GX2SurfaceFormat format, u32 numMips)
{
    GX2Texture texture;
    GX2InitTexture(&texture, width, height, 0, numMips, format, GX2_SURFACE_DIM_2D);
    return (
        texture.surface.alignment +
        FFLiRoundUp(texture.surface.imageSize, texture.surface.alignment) +
        FFLiRoundUp(texture.surface.mipSize, texture.surface.alignment)
    );
}

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, GX2SurfaceFormat format, u32 numMips, FFLiBufferAllocator* pAllocator)
{
    std::memset(pRenderTexture, 0, sizeof(FFLiRenderTexture));

    GX2InitTexture(&pRenderTexture->gx2Texture, width, height, 0, numMips, format, GX2_SURFACE_DIM_2D);

    void* imagePtr = FFLiAllocateBufferAllocator(pAllocator, pRenderTexture->gx2Texture.surface.imageSize, pRenderTexture->gx2Texture.surface.alignment);

    void* mipPtr =
        pRenderTexture->gx2Texture.surface.mipSize != 0
            ? FFLiAllocateBufferAllocator(pAllocator, pRenderTexture->gx2Texture.surface.mipSize, pRenderTexture->gx2Texture.surface.alignment)
            : NULL;

    pRenderTexture->gx2Texture.surface.imagePtr = imagePtr;

    if (pRenderTexture->gx2Texture.surface.numMips > 1 && mipPtr == NULL)
        pRenderTexture->gx2Texture.surface.mipPtr = (u8*)imagePtr + pRenderTexture->gx2Texture.surface.mipOffset[0];
    else
        pRenderTexture->gx2Texture.surface.mipPtr = mipPtr;
}

void FFLiInitByBufferRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, GX2SurfaceFormat format, u32 numMips, FFLiRenderTextureBuffer* pRenderTextureBuffer)
{
    FFLiBufferAllocator allocator;
    allocator.Init(pRenderTextureBuffer->pBuffer, pRenderTextureBuffer->size);
    FFLiInitRenderTexture(pRenderTexture, width, height, format, numMips, &allocator);
}

void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    FFLiInvalidateTexture(&pRenderTexture->gx2Texture);
}

void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, void* pDepthBuffer, u32 viewMip, GX2SurfaceFormat format, const FFLiShaderCallback* pCallback)
{
    FFLiSetupRenderTexture(&pRenderTexture->gx2Texture, pClearColor, pDepthBuffer, viewMip, format, pCallback);
}

void FFLiSetupRenderTexture(GX2Texture* pGX2Texture, const FFLColor* pClearColor, void* pDepthBuffer, u32 viewMip, GX2SurfaceFormat format, const FFLiShaderCallback* pCallback)
{
    GX2ColorBuffer colorBuffer;
    std::memset(&colorBuffer, 0, sizeof(GX2ColorBuffer));

    GX2DepthBuffer depthBuffer;
    std::memset(&depthBuffer, 0, sizeof(GX2DepthBuffer));

    GX2InitColorBuffer(&colorBuffer, pGX2Texture->surface.width, pGX2Texture->surface.height, format, GX2_AA_MODE_1X);

    colorBuffer.surface.numMips = pGX2Texture->surface.numMips;
    colorBuffer.viewMip = viewMip;
    GX2CalcSurfaceSizeAndAlignment(&colorBuffer.surface);
    GX2InitColorBufferRegs(&colorBuffer);

    colorBuffer.surface.imagePtr = pGX2Texture->surface.imagePtr;
    colorBuffer.surface.mipPtr = pGX2Texture->surface.mipPtr;
    GX2SetColorBuffer(&colorBuffer, GX2_RENDER_TARGET_0);

    if (pDepthBuffer != NULL)
    {
        GX2InitDepthBuffer(&depthBuffer, pGX2Texture->surface.width, pGX2Texture->surface.height, GX2_SURFACE_FORMAT_TCD_R16_UNORM, GX2_AA_MODE_1X);

        depthBuffer.surface.imagePtr = FFLiRoundUpPtr(pDepthBuffer, depthBuffer.surface.alignment);
        GX2SetDepthBuffer(&depthBuffer);
    }
    else
    {
        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_NEVER);
    }

    GX2SetViewport(
        0.0f,
        0.0f,
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.width, viewMip)),
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.height, viewMip)),
        0.0f,
        1.0f
    );
    GX2SetScissor(
        0.0f,
        0.0f,
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.width, viewMip)),
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.height, viewMip))
    );

    if (pClearColor != NULL)
    {
        GX2ClearColor(&colorBuffer, pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a);
        if (pDepthBuffer)
            GX2ClearDepthStencil(&depthBuffer, GX2_CLEAR_BOTH);
    }

    pCallback->CallSetContextState();
}

void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    FFLiInvalidate(GX2_INVALIDATE_COLOR_BUFFER, pRenderTexture->gx2Texture.surface.imagePtr, pRenderTexture->gx2Texture.surface.imageSize);
    if (pRenderTexture->gx2Texture.surface.mipPtr != NULL)
        FFLiInvalidate(GX2_INVALIDATE_COLOR_BUFFER, pRenderTexture->gx2Texture.surface.mipPtr, pRenderTexture->gx2Texture.surface.mipSize);
}
