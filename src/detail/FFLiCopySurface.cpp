#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/FFLiMipMapUtil.h>

#include <nn/ffl/detail/FFLiCopySurface.h>

#include <gfx/rio_Window.h>

#include <gx2/mem.h>
#include <gx2/registers.h>

FFLiCopySurface::FFLiCopySurface()
{
    m_Shader.SetupCPU();
    m_Drawer.SetupCPU();
}

FFLiCopySurface::~FFLiCopySurface()
{
}

void FFLiCopySurface::SetupGPU()
{
    m_Shader.SetupGPU();
    m_Drawer.SetupGPU();
}

void FFLiCopySurface::Begin()
{
    GX2SetContextState(NULL);
    GX2Invalidate(GX2_INVALIDATE_MODE_SHADER, NULL, 0xFFFFFFFF);

    GX2SetDefaultState();
    GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_FUNC_NEVER);
    GX2SetTargetChannelMasks(
        GX2_CHANNEL_MASK_RGBA,
        (GX2ChannelMask)0,
        (GX2ChannelMask)0,
        (GX2ChannelMask)0,
        (GX2ChannelMask)0,
        (GX2ChannelMask)0,
        (GX2ChannelMask)0,
        (GX2ChannelMask)0
    );

    m_Shader.Bind();
    m_Drawer.SetAttributeBuffer(0, 1);
}

void FFLiCopySurface::End()
{
    GX2SetContextState(rio::Window::instance()->getNativeWindow().getContextState());
    GX2Invalidate(GX2_INVALIDATE_MODE_SHADER, NULL, 0xFFFFFFFF);
}

void FFLiCopySurface::Execute(GX2Surface* pDstSurface, u32 dstMipLevel, const GX2Surface* pSrcSurface, u32 srcMipLevel)
{
    SetupSrcSurface(pSrcSurface, srcMipLevel);
    SetupDstSurface(pDstSurface, dstMipLevel);

    {
        if (pDstSurface->imageSize != 0)
            GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, pDstSurface->image, pDstSurface->imageSize);

        if (pDstSurface->mipmapSize != 0)
            GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, pDstSurface->mipmaps, pDstSurface->mipmapSize);
    }

    m_Drawer.Draw();

    {
        if (pDstSurface->imageSize != 0)
            GX2Invalidate(GX2_INVALIDATE_MODE_COLOR_BUFFER, pDstSurface->image, pDstSurface->imageSize);

        if (pDstSurface->mipmapSize != 0)
            GX2Invalidate(GX2_INVALIDATE_MODE_COLOR_BUFFER, pDstSurface->mipmaps, pDstSurface->mipmapSize);
    }
}

void FFLiCopySurface::SetupSrcSurface(const GX2Surface* pSurface, u32 mipLevel)
{
    GX2Texture texture;

    texture.surface.dim = pSurface->dim;
    texture.surface.width = pSurface->width;
    texture.surface.height = pSurface->height;
    texture.surface.depth = pSurface->depth;
    texture.surface.mipLevels = pSurface->mipLevels;
    texture.surface.format = pSurface->format;
    texture.surface.aa = GX2_AA_MODE1X;
    texture.surface.use = GX2_SURFACE_USE_TEXTURE;
    texture.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    texture.surface.swizzle = 0;
    GX2CalcSurfaceSizeAndAlignment(&texture.surface);

    texture.viewFirstMip = 0;
    texture.viewNumMips = pSurface->mipLevels;
    texture.viewFirstSlice = 0;
    texture.viewNumSlices = pSurface->depth;
    texture.compMap = rio::TextureFormatUtil::getDefaultCompMap(rio::TextureFormat(pSurface->format));
    GX2InitTextureRegs(&texture);

    texture.surface.image = pSurface->image;
    if (texture.surface.mipLevels > 1 && pSurface->mipmaps == nullptr)
        texture.surface.mipmaps = (u8*)pSurface->image + texture.surface.mipLevelOffset[0];
    else
        texture.surface.mipmaps = pSurface->mipmaps;

    m_Shader.SetTexture(&texture, mipLevel);
}

void FFLiCopySurface::SetupDstSurface(GX2Surface* pSurface, u32 mipLevel)
{
    GX2ColorBuffer colorBuffer;

    colorBuffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    colorBuffer.surface.width = pSurface->width;
    colorBuffer.surface.height = pSurface->height;
    colorBuffer.surface.depth = 1;
    colorBuffer.surface.mipLevels = pSurface->mipLevels;
    colorBuffer.surface.mipmaps = nullptr;
    colorBuffer.surface.format = pSurface->format;
    colorBuffer.surface.aa = GX2_AA_MODE1X;
    colorBuffer.surface.use = GX2_SURFACE_USE_COLOR_BUFFER | GX2_SURFACE_USE_TEXTURE;
    colorBuffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    colorBuffer.surface.swizzle = 0;
    GX2CalcSurfaceSizeAndAlignment(&colorBuffer.surface);

    colorBuffer.viewMip = mipLevel;
    colorBuffer.viewFirstSlice = 0;
    colorBuffer.viewNumSlices = 1;
    GX2InitColorBufferRegs(&colorBuffer);

    colorBuffer.surface.image = pSurface->image;
    colorBuffer.surface.mipmaps = pSurface->mipmaps;
    GX2SetColorBuffer(&colorBuffer, GX2_RENDER_TARGET_0);

    GX2SetViewport(
        0.0f,
        0.0f,
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.width, mipLevel)),
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.height, mipLevel)),
        0.0f,
        1.0f
    );
    GX2SetScissor(
        0.0f,
        0.0f,
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.width, mipLevel)),
        f32(FFLiGetMipMapLevelSize(colorBuffer.surface.height, mipLevel))
    );
}

#endif // RIO_IS_CAFE
