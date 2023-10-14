#include <nn/ffl/FFLiInvalidate.h>
#include <nn/ffl/FFLiMipMapUtil.h>
#include <nn/ffl/FFLiTemp.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>
#include <nn/ffl/detail/FFLiCopySurface.h>

FFLiCopySurface::FFLiCopySurface(FFLiBufferAllocator* pAllocator)
{
    u32 size = GetBufferSize();
    void* pBuffer = pAllocator->Allocate(size);

    FFLiBufferAllocator allocator;
    allocator.Init(pBuffer, size);

    m_Shader.SetupCPU(&allocator);
    m_Drawer.SetupCPU(&allocator);
}

FFLiCopySurface::~FFLiCopySurface()
{
}

u32 FFLiCopySurface::GetBufferSize()
{
    u32 ret  = FFLiCopySurfaceShader::GetBufferSize();
    ret     += FFLiCopySurfaceDrawer::GetBufferSize();

    return ret;
}

void FFLiCopySurface::SetupGPU()
{
    m_Shader.SetupGPU();
    m_Drawer.SetupGPU();
}

void FFLiCopySurface::Begin()
{
    FFLiTempSetContextState(NULL);
    GX2SetDefaultState();
    GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_NEVER);
    GX2SetTargetChannelMasks(
        GX2_CHANNEL_MASK_RGBA,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE
    );

    m_Shader.Bind();
    m_Drawer.SetAttributeBuffer(0, 1);
}

void FFLiCopySurface::Execute(GX2Surface* pDstSurface, u32 dstMipLevel, const GX2Surface* pSrcSurface, u32 srcMipLevel)
{
    SetupSrcSurface(pSrcSurface, srcMipLevel);
    SetupDstSurface(pDstSurface, dstMipLevel);

    FFLiInvalidateSurface(pDstSurface, GX2_INVALIDATE_TEXTURE);
    m_Drawer.Draw();
    FFLiInvalidateSurface(pDstSurface, GX2_INVALIDATE_COLOR_BUFFER);
}

bool FFLiCopySurface::CanInitCharModel(bool isSetupGPU, bool compressTexture) const
{
    return isSetupGPU;
}

void FFLiCopySurface::SetupSrcSurface(const GX2Surface* pSurface, u32 mipLevel)
{
    GX2Texture texture;
    GX2InitTexture(&texture, pSurface->width, pSurface->height, pSurface->depth, pSurface->numMips, pSurface->format, pSurface->dim);
    GX2InitTexturePtrs(&texture, pSurface->imagePtr, pSurface->mipPtr);
    m_Shader.SetTexture(&texture, mipLevel);
}

void FFLiCopySurface::SetupDstSurface(GX2Surface* pSurface, u32 mipLevel)
{
    GX2ColorBuffer colorBuffer;
    GX2InitColorBuffer(&colorBuffer, pSurface->width, pSurface->height, pSurface->format, GX2_AA_MODE_1X);

    colorBuffer.surface.numMips = pSurface->numMips;
    colorBuffer.viewMip = mipLevel;
    GX2CalcSurfaceSizeAndAlignment(&colorBuffer.surface);
    GX2InitColorBufferRegs(&colorBuffer);

    colorBuffer.surface.imagePtr = pSurface->imagePtr;
    colorBuffer.surface.mipPtr = pSurface->mipPtr;
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
