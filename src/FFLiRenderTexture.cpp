#include <nn/ffl/FFLColor.h>

#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiRenderTexture.h>
#include <nn/ffl/FFLiRenderTextureBuffer.h>
#include <nn/ffl/FFLiShaderCallback.h>
#include <nn/ffl/FFLiTexture.h>
#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

#include <detail/aglTextureDataUtil.h>
#include <gpu/rio_RenderState.h>

#if RIO_IS_CAFE
#include <cafe/gx2.h>
#endif // RIO_IS_CAFE

#include <cstring>

u32 FFLiGetBufferRenderTexture(u32 width, u32 height, rio::TextureFormat format, u32 numMips)
{
#if RIO_IS_CAFE
    GX2Texture texture;
    GX2InitTexture(&texture, width, height, 0, numMips, (GX2SurfaceFormat)format, GX2_SURFACE_DIM_2D);
    return (
        texture.surface.alignment +
        FFLiRoundUp(texture.surface.imageSize, texture.surface.alignment) +
        FFLiRoundUp(texture.surface.mipSize, texture.surface.alignment)
    );
#else
    return 0;
#endif // RIO_IS_CAFE
}

void FFLiInitRenderTexture(FFLiRenderTexture* pRenderTexture, u32 width, u32 height, rio::TextureFormat format, u32 numMips, FFLiBufferAllocator* pAllocator)
{
  //std::memset(pRenderTexture, 0, sizeof(FFLiRenderTexture));
    pRenderTexture->textureData = agl::TextureData();
    pRenderTexture->renderBuffer.destroy();

    pRenderTexture->textureData.initialize(
        agl::detail::TextureDataUtil::convFormatGX2ToAGL((GX2SurfaceFormat)format, true, false),
        width,
        height,
        numMips
    );

#if RIO_IS_CAFE

    pRenderTexture->textureData.setImagePtr(FFLiAllocateBufferAllocator(pAllocator, pRenderTexture->textureData.getImageByteSize(), pRenderTexture->textureData.getAlignment()));

    if (pRenderTexture->textureData.getMipLevelNum() > 1)
        pRenderTexture->textureData.setMipPtr(FFLiAllocateBufferAllocator(pAllocator, pRenderTexture->textureData.getMipByteSize(), pRenderTexture->textureData.getAlignment()));
    else
        pRenderTexture->textureData.setMipPtr(nullptr);

#elif RIO_IS_WIN

    const auto& handle = std::make_shared<agl::TextureHandle>();
    handle->bind();

    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, pRenderTexture->textureData.getMipLevelNum() - 1));

    RIO_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    const rio::NativeTextureFormat& native_format = pRenderTexture->textureData.getNativeTextureFormat();

    switch (pRenderTexture->textureData.getTextureFormat())
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
                pRenderTexture->textureData.getWidth(),
                pRenderTexture->textureData.getHeight(),
                0,
                pRenderTexture->textureData.getImageByteSize(),
                nullptr
            ));

            if (pRenderTexture->textureData.getMipLevelNum() > 1)
            {
                for (u32 i = 0; i < pRenderTexture->textureData.getMipLevelNum() - 2; i++)
                    RIO_GL_CALL(glCompressedTexImage2D(
                        GL_TEXTURE_2D,
                        i + 1,
                        native_format.internalformat,
                        pRenderTexture->textureData.getWidth(i + 1),
                        pRenderTexture->textureData.getHeight(i + 1),
                        0,
                        pRenderTexture->textureData.getMipLevelByteSize(i + 1),
                        nullptr
                    ));

                RIO_GL_CALL(glCompressedTexImage2D(
                    GL_TEXTURE_2D,
                    (pRenderTexture->textureData.getMipLevelNum() - 2) + 1,
                    native_format.internalformat,
                    pRenderTexture->textureData.getWidth((pRenderTexture->textureData.getMipLevelNum() - 2) + 1),
                    pRenderTexture->textureData.getHeight((pRenderTexture->textureData.getMipLevelNum() - 2) + 1),
                    0,
                    pRenderTexture->textureData.getMipLevelByteSize((pRenderTexture->textureData.getMipLevelNum() - 2) + 1),
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
                pRenderTexture->textureData.getWidth(),
                pRenderTexture->textureData.getHeight(),
                0,
                pRenderTexture->textureData.getImageByteSize(),
                nullptr
            ));

            if (pRenderTexture->textureData.getMipLevelNum() > 1)
            {
                for (u32 i = 0; i < pRenderTexture->textureData.getMipLevelNum() - 2; i++)
                    RIO_GL_CALL(glCompressedTexImage2DARB(
                        GL_TEXTURE_2D,
                        i + 1,
                        native_format.internalformat,
                        pRenderTexture->textureData.getWidth(i + 1),
                        pRenderTexture->textureData.getHeight(i + 1),
                        0,
                        pRenderTexture->textureData.getMipLevelByteSize(i + 1),
                        nullptr
                    ));

                RIO_GL_CALL(glCompressedTexImage2DARB(
                    GL_TEXTURE_2D,
                    (pRenderTexture->textureData.getMipLevelNum() - 2) + 1,
                    native_format.internalformat,
                    pRenderTexture->textureData.getWidth((pRenderTexture->textureData.getMipLevelNum() - 2) + 1),
                    pRenderTexture->textureData.getHeight((pRenderTexture->textureData.getMipLevelNum() - 2) + 1),
                    0,
                    pRenderTexture->textureData.getMipLevelByteSize((pRenderTexture->textureData.getMipLevelNum() - 2) + 1),
                    nullptr
                ));
            }
        }
        break;
    default:
        {
            RIO_GL_CALL(glTexStorage2D(
                GL_TEXTURE_2D,
                pRenderTexture->textureData.getMipLevelNum(),
                native_format.internalformat,
                pRenderTexture->textureData.getWidth(),
                pRenderTexture->textureData.getHeight()
            ));
        }
        break;
    }

    pRenderTexture->textureData.setHandle(handle);

#endif
}

void FFLiInvalidateRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    pRenderTexture->textureData.invalidateGPUCache();
}

void FFLiSetupRenderTexture(FFLiRenderTexture* pRenderTexture, const FFLColor* pClearColor, agl::TextureData* pDepthBuffer, u32 mipLevel, const FFLiShaderCallback* pCallback)
{
    pRenderTexture->colorTarget.applyTextureData(pRenderTexture->textureData);
    pRenderTexture->colorTarget.setMipLevel(mipLevel);
    pRenderTexture->renderBuffer.setRenderTargetColor(&pRenderTexture->colorTarget);

    if (pDepthBuffer != NULL)
    {
        pRenderTexture->depthTarget.applyTextureData(*pDepthBuffer);
        pRenderTexture->renderBuffer.setRenderTargetDepth(&pRenderTexture->depthTarget);
    }
    else
    {
        pRenderTexture->renderBuffer.setRenderTargetDepth(nullptr);

        rio::RenderState renderState;
        renderState.setDepthEnable(false, false);
        renderState.applyDepthAndStencilTest();
    }

    pRenderTexture->renderBuffer.setSize(
        FFLiGetMipMapLevelSize(pRenderTexture->textureData.getWidth(), mipLevel),
        FFLiGetMipMapLevelSize(pRenderTexture->textureData.getHeight(), mipLevel)
    );

    pRenderTexture->renderBuffer.bind();

    if (pClearColor != NULL)
    {
#if RIO_IS_CAFE
        GX2ClearColor(&pRenderTexture->colorTarget.getInnerBuffer(), pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a);
#elif RIO_IS_WIN
        RIO_GL_CALL(glClearColor(pClearColor->r, pClearColor->g, pClearColor->b, pClearColor->a));
        RIO_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
#endif

        if (pDepthBuffer)
        {
#if RIO_IS_CAFE
            GX2ClearDepthStencil(&pRenderTexture->depthTarget.getInnerBuffer(), GX2_CLEAR_BOTH);
#elif RIO_IS_WIN
            RIO_GL_CALL(glDepthMask(GL_TRUE));
            RIO_GL_CALL(glClearDepth(1.0f));
            RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
#endif
        }
    }

    pCallback->CallSetContextState();
}

void FFLiFlushRenderTexture(FFLiRenderTexture* pRenderTexture)
{
    RIO_ASSERT(pRenderTexture->renderBuffer.getRenderTargetColor());
    pRenderTexture->renderBuffer.getRenderTargetColor()->invalidateGPUCache();
}
