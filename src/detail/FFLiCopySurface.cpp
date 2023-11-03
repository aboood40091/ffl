#include <nn/ffl/FFLiMipMapUtil.h>

#include <nn/ffl/detail/FFLiCopySurface.h>

#if RIO_IS_CAFE
#include <gx2/registers.h>
#endif // RIO_IS_CAFE

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
    m_Drawer.SetupGPU(0, 1);
}

void FFLiCopySurface::Begin()
{
#if RIO_IS_CAFE
    GX2SetContextState(nullptr);
    GX2Invalidate(GX2_INVALIDATE_SHADER, NULL, 0xFFFFFFFF);

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
#endif // RIO_IS_CAFE

    m_Shader.Bind();
    m_Drawer.SetAttributeBuffer();
}

void FFLiCopySurface::Execute(agl::TextureData* pTextureData, u32 dstMipLevel, u32 srcMipLevel)
{
    RIO_ASSERT(pTextureData);

    m_Shader.SetTexture(*pTextureData, srcMipLevel);

    m_ColorTarget.applyTextureData(*pTextureData);
    m_ColorTarget.setMipLevel(dstMipLevel);

    m_RenderBuffer.setRenderTargetColor(&m_ColorTarget);
    m_RenderBuffer.setRenderTargetDepthNull();
    m_RenderBuffer.setSize(
        FFLiGetMipMapLevelSize(pTextureData->getWidth(), dstMipLevel),
        FFLiGetMipMapLevelSize(pTextureData->getHeight(), dstMipLevel)
    );
    m_RenderBuffer.bind();

    pTextureData->invalidateGPUCache();
    m_Drawer.Draw();
    m_ColorTarget.invalidateGPUCache();
}

bool FFLiCopySurface::CanInitCharModel(bool isSetupGPU) const
{
    return isSetupGPU;
}
