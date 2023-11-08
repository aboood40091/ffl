#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <gpu/rio_RenderState.h>

#if RIO_IS_CAFE
#include <gfx/rio_Window.h>
#include <misc/rio_MemUtil.h>

#include <gx2/clear.h>
#include <gx2/mem.h>
#endif // RIO_IS_CAFE

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, rio::TextureFormat format, u32 numMips)
{
    rio::Texture2D* pTexture2D = new rio::Texture2D(
        format,
        width,
        height,
        numMips
    );
    rio::RenderBuffer* pRenderBuffer = new rio::RenderBuffer;
    rio::RenderTargetColor* pColorTarget = new rio::RenderTargetColor;
    rio::RenderTargetDepth* pDepthTarget = new rio::RenderTargetDepth;

    pRenderTexture->pTexture2D = pTexture2D;
    pRenderTexture->pRenderBuffer = pRenderBuffer;
    pRenderTexture->pColorTarget = pColorTarget;
    pRenderTexture->pDepthTarget = pDepthTarget;
}

void FFLiDeleteRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    rio::Texture2D* pTexture2D = pRenderTexture->pTexture2D;
    rio::RenderBuffer* pRenderBuffer = pRenderTexture->pRenderBuffer;
    rio::RenderTargetColor* pColorTarget = pRenderTexture->pColorTarget;
    rio::RenderTargetDepth* pDepthTarget = pRenderTexture->pDepthTarget;

    pRenderTexture->pTexture2D = nullptr;
    pRenderTexture->pRenderBuffer = nullptr;
    pRenderTexture->pColorTarget = nullptr;
    pRenderTexture->pDepthTarget = nullptr;

    delete pRenderBuffer;
    delete pColorTarget;
    delete pDepthTarget;

    delete pTexture2D;
}

void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture)
{
#if RIO_IS_CAFE
    const GX2Texture& texture = pRenderTexture->pTexture2D->getNativeTexture();

    if (texture.surface.image)
        GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, texture.surface.image, texture.surface.imageSize);

    if (texture.surface.mipmaps)
        GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, texture.surface.mipmaps, texture.surface.mipmapSize);
#endif // RIO_IS_CAFE
}

void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, rio::Texture2D* pDepthBuffer, u32 mipLevel, const FFLiShaderCallback* pCallback)
{
    rio::Texture2D* pTexture2D = pRenderTexture->pTexture2D;
    rio::RenderBuffer* pRenderBuffer = pRenderTexture->pRenderBuffer;
    rio::RenderTargetColor* pColorTarget = pRenderTexture->pColorTarget;
    rio::RenderTargetDepth* pDepthTarget = pRenderTexture->pDepthTarget;

    pColorTarget->linkTexture2D(*pTexture2D);
    pColorTarget->setMipLevel(mipLevel);
    pRenderBuffer->setRenderTargetColor(pColorTarget);

    if (pDepthBuffer != NULL)
    {
        pDepthTarget->linkTexture2D(*pDepthBuffer);
        pRenderBuffer->setRenderTargetDepth(pDepthTarget);
    }
    else
    {
        pRenderBuffer->setRenderTargetDepth(nullptr);

        rio::RenderState renderState;
        renderState.setDepthEnable(false, false);
        renderState.applyDepthAndStencilTest();
    }

    pRenderBuffer->setSize(
        FFLiGetMipMapLevelSize(pTexture2D->getWidth(), mipLevel),
        FFLiGetMipMapLevelSize(pTexture2D->getHeight(), mipLevel)
    );

    pRenderBuffer->bind();

    if (pClearColor != NULL)
    {
#if RIO_IS_CAFE
        GX2ColorBuffer* pGX2ColorBuffer = &pColorTarget->getNativeColorBuffer();
        GX2ClearColor(pGX2ColorBuffer, pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a);
#elif RIO_IS_WIN
        RIO_GL_CALL(glClearColor(pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a));
        RIO_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
#endif

        if (pDepthBuffer)
        {
#if RIO_IS_CAFE
            GX2DepthBuffer* pGX2DepthBuffer = &pDepthTarget->getNativeDepthBuffer();
            GX2ClearDepthStencilEx(pGX2DepthBuffer, pGX2DepthBuffer->depthClear, pGX2DepthBuffer->stencilClear, GX2_CLEAR_FLAGS_BOTH);
#elif RIO_IS_WIN
            RIO_GL_CALL(glDepthMask(GL_TRUE));
            RIO_GL_CALL(glClearDepth(1.0f));
            RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
#endif
        }

#if RIO_IS_CAFE
        GX2SetContextState(rio::Window::instance()->getNativeWindow().getContextState());
        GX2Invalidate(GX2_INVALIDATE_MODE_SHADER, NULL, 0xFFFFFFFF);
#endif // RIO_IS_CAFE
    }
}

void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    rio::RenderBuffer* pRenderBuffer = pRenderTexture->pRenderBuffer;
    RIO_ASSERT(pRenderBuffer->getRenderTargetColor());
    pRenderBuffer->getRenderTargetColor()->invalidateGPUCache();
}
