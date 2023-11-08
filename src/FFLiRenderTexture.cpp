#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <detail/aglTextureDataUtil.h>
#include <gpu/rio_RenderState.h>

#if RIO_IS_CAFE
#include <gfx/rio_Window.h>
#include <misc/rio_MemUtil.h>

#include <gx2/clear.h>
#include <gx2/mem.h>
#endif // RIO_IS_CAFE

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, rio::TextureFormat format, u32 numMips)
{
    agl::TextureData* pTextureData = new agl::TextureData;
    agl::RenderBuffer* pRenderBuffer = new agl::RenderBuffer;
    agl::RenderTargetColor* pColorTarget = new agl::RenderTargetColor;
    agl::RenderTargetDepth* pDepthTarget = new agl::RenderTargetDepth;

    pRenderTexture->pTextureData = pTextureData;
    pRenderTexture->pRenderBuffer = pRenderBuffer;
    pRenderTexture->pColorTarget = pColorTarget;
    pRenderTexture->pDepthTarget = pDepthTarget;

    pTextureData->initialize(
        agl::detail::TextureDataUtil::convFormatGX2ToAGL((GX2SurfaceFormat)format, true, false),
        width,
        height,
        numMips
    );

#if RIO_IS_CAFE

    pTextureData->setImagePtr(rio::MemUtil::alloc(pTextureData->getImageByteSize(), pTextureData->getAlignment()));

    if (pTextureData->getMipLevelNum() > 1)
        pTextureData->setMipPtr(rio::MemUtil::alloc(pTextureData->getMipByteSize(), pTextureData->getAlignment()));
    else
        pTextureData->setMipPtr(nullptr);

#elif RIO_IS_WIN

    const auto& handle = std::make_shared<agl::TextureHandle>();
    handle->bind();

    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, pTextureData->getMipLevelNum() - 1));

    RIO_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    const rio::NativeTextureFormat& native_format = pTextureData->getNativeTextureFormat();

    switch (pTextureData->getTextureFormat())
    {
    case agl::cTextureFormat_BC1_uNorm:
    case agl::cTextureFormat_BC2_uNorm:
    case agl::cTextureFormat_BC3_uNorm:
    case agl::cTextureFormat_BC4_uNorm:
    case agl::cTextureFormat_BC4_sNorm:
    case agl::cTextureFormat_BC5_uNorm:
    case agl::cTextureFormat_BC5_sNorm:
        {
            RIO_GL_CALL(glCompressedTexImage2D(
                GL_TEXTURE_2D,
                0,
                native_format.internalformat,
                pTextureData->getWidth(),
                pTextureData->getHeight(),
                0,
                pTextureData->getImageByteSize(),
                nullptr
            ));

            if (pTextureData->getMipLevelNum() > 1)
            {
                for (u32 i = 0; i < pTextureData->getMipLevelNum() - 2; i++)
                    RIO_GL_CALL(glCompressedTexImage2D(
                        GL_TEXTURE_2D,
                        i + 1,
                        native_format.internalformat,
                        pTextureData->getWidth(i + 1),
                        pTextureData->getHeight(i + 1),
                        0,
                        pTextureData->getMipLevelByteSize(i + 1),
                        nullptr
                    ));

                RIO_GL_CALL(glCompressedTexImage2D(
                    GL_TEXTURE_2D,
                    (pTextureData->getMipLevelNum() - 2) + 1,
                    native_format.internalformat,
                    pTextureData->getWidth((pTextureData->getMipLevelNum() - 2) + 1),
                    pTextureData->getHeight((pTextureData->getMipLevelNum() - 2) + 1),
                    0,
                    pTextureData->getMipLevelByteSize((pTextureData->getMipLevelNum() - 2) + 1),
                    nullptr
                ));
            }
        }
        break;
    case agl::cTextureFormat_BC1_SRGB:
    case agl::cTextureFormat_BC2_SRGB:
    case agl::cTextureFormat_BC3_SRGB:
        {
            RIO_GL_CALL(glCompressedTexImage2DARB(
                GL_TEXTURE_2D,
                0,
                native_format.internalformat,
                pTextureData->getWidth(),
                pTextureData->getHeight(),
                0,
                pTextureData->getImageByteSize(),
                nullptr
            ));

            if (pTextureData->getMipLevelNum() > 1)
            {
                for (u32 i = 0; i < pTextureData->getMipLevelNum() - 2; i++)
                    RIO_GL_CALL(glCompressedTexImage2DARB(
                        GL_TEXTURE_2D,
                        i + 1,
                        native_format.internalformat,
                        pTextureData->getWidth(i + 1),
                        pTextureData->getHeight(i + 1),
                        0,
                        pTextureData->getMipLevelByteSize(i + 1),
                        nullptr
                    ));

                RIO_GL_CALL(glCompressedTexImage2DARB(
                    GL_TEXTURE_2D,
                    (pTextureData->getMipLevelNum() - 2) + 1,
                    native_format.internalformat,
                    pTextureData->getWidth((pTextureData->getMipLevelNum() - 2) + 1),
                    pTextureData->getHeight((pTextureData->getMipLevelNum() - 2) + 1),
                    0,
                    pTextureData->getMipLevelByteSize((pTextureData->getMipLevelNum() - 2) + 1),
                    nullptr
                ));
            }
        }
        break;
    default:
        {
            RIO_GL_CALL(glTexStorage2D(
                GL_TEXTURE_2D,
                pTextureData->getMipLevelNum(),
                native_format.internalformat,
                pTextureData->getWidth(),
                pTextureData->getHeight()
            ));
        }
        break;
    }

    pTextureData->setHandle(handle);

#endif
}

void FFLiDeleteRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    agl::TextureData* pTextureData = pRenderTexture->pTextureData;
    agl::RenderBuffer* pRenderBuffer = pRenderTexture->pRenderBuffer;
    agl::RenderTargetColor* pColorTarget = pRenderTexture->pColorTarget;
    agl::RenderTargetDepth* pDepthTarget = pRenderTexture->pDepthTarget;

    pRenderTexture->pTextureData = nullptr;
    pRenderTexture->pRenderBuffer = nullptr;
    pRenderTexture->pColorTarget = nullptr;
    pRenderTexture->pDepthTarget = nullptr;

    delete pRenderBuffer;
    delete pColorTarget;
    delete pDepthTarget;

#if RIO_IS_CAFE
    rio::MemUtil::free(pTextureData->getImagePtr());
    if (pTextureData->getMipLevelNum() > 1)
        rio::MemUtil::free(pTextureData->getMipPtr());
#endif // RIO_IS_CAFE

    delete pTextureData;
}

void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    pRenderTexture->pTextureData->invalidateGPUCache();
}

void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, agl::TextureData* pDepthBuffer, u32 mipLevel, const FFLiShaderCallback* pCallback)
{
    agl::TextureData* pTextureData = pRenderTexture->pTextureData;
    agl::RenderBuffer* pRenderBuffer = pRenderTexture->pRenderBuffer;
    agl::RenderTargetColor* pColorTarget = pRenderTexture->pColorTarget;
    agl::RenderTargetDepth* pDepthTarget = pRenderTexture->pDepthTarget;

    pColorTarget->applyTextureData(*pTextureData);
    pColorTarget->setMipLevel(mipLevel);
    pRenderBuffer->setRenderTargetColor(pColorTarget);

    if (pDepthBuffer != NULL)
    {
        pDepthTarget->applyTextureData(*pDepthBuffer);
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
        FFLiGetMipMapLevelSize(pTextureData->getWidth(), mipLevel),
        FFLiGetMipMapLevelSize(pTextureData->getHeight(), mipLevel)
    );

    pRenderBuffer->bind();

    if (pClearColor != NULL)
    {
#if RIO_IS_CAFE
        GX2ClearColor(&pColorTarget->getInnerBuffer(), pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a);
#elif RIO_IS_WIN
        RIO_GL_CALL(glClearColor(pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a));
        RIO_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
#endif

        if (pDepthBuffer)
        {
#if RIO_IS_CAFE
            GX2DepthBuffer* pGX2DepthBuffer = &pDepthTarget->getInnerBuffer();
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
    agl::RenderBuffer* pRenderBuffer = pRenderTexture->pRenderBuffer;
    RIO_ASSERT(pRenderBuffer->getRenderTargetColor());
    pRenderBuffer->getRenderTargetColor()->invalidateGPUCache();
}
