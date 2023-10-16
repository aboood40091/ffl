#include <nn/ffl/detail/FFLiResourceTexture.h>

GX2SurfaceFormat FFLiGetResourceSurfaceFormat(FFLiTextureFormat textureFormat)
{
    switch (textureFormat)
    {
    case FFLI_TEXTURE_FORMAT_A8:
        return GX2_SURFACE_FORMAT_TC_R8_UNORM;
    case FFLI_TEXTURE_FORMAT_AL8:
        return GX2_SURFACE_FORMAT_TC_R8_G8_UNORM;
    case FFLI_TEXTURE_FORMAT_RGBA8:
        return GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM;
    }
    return GX2_SURFACE_FORMAT_TC_R8_UNORM;
}

GX2SurfaceFormat FFLiResourceTextureFooter::SurfaceFormat() const
{
    return FFLiGetResourceSurfaceFormat(FFLiTextureFormat(m_TextureFormat));
}

FFLiResourceTextureFooter& FFLiResourceTextureFooter::GetFooterImpl(const void* pData, u32 size)
{
    return ((FFLiResourceTextureFooter*)((u8*)pData + size))[-1];
}

void* FFLiResourceTextureFooter::GetImagePtrImpl(u32 size) const
{
    return (u8*)(this + 1) - size;
}

void* FFLiResourceTextureFooter::GetMipPtrImpl(u32 size) const
{
    return m_MipOffset != 0 ? ((u8*)(this + 1) - size + m_MipOffset) : NULL;
}
