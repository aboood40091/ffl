#include <nn/ffl/detail/FFLiCompressUtil.h>

#include <cstring>

namespace {

void SetupColorBuffer(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel, GX2SurfaceFormat format);

}

u32 FFLiCalculateUnitTextureResolution(u32 dimension, u32 mipLevel)
{
    u32 mipDimension = dimension >> mipLevel;
    mipDimension += (4 - mipDimension) % 4;
    return (mipDimension << mipLevel) / 4;
}

void FFLiSetupColorBufferForBC3(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel)
{
    SetupColorBuffer(pColorBuffer, pTexture, mipLevel, GX2_SURFACE_FORMAT_TC_R32_G32_B32_A32_UINT);
}

void FFLiSetupColorBufferForBC1(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel)
{
    SetupColorBuffer(pColorBuffer, pTexture, mipLevel, GX2_SURFACE_FORMAT_TC_R16_G16_B16_A16_UINT);
}

namespace {

void SetupColorBuffer(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel, GX2SurfaceFormat format)
{
    u32 width = FFLiCalculateUnitTextureResolution(pTexture->surface.width,  mipLevel);
    u32 height = FFLiCalculateUnitTextureResolution(pTexture->surface.height, mipLevel);

    std::memset(pColorBuffer, 0, sizeof(GX2ColorBuffer));
    GX2InitColorBuffer(pColorBuffer, width, height, format, GX2_AA_MODE_1X);

    pColorBuffer->surface.numMips = pTexture->surface.numMips;
    pColorBuffer->surface.imageSize = pTexture->surface.imageSize;
    pColorBuffer->surface.mipSize = pTexture->surface.mipSize;
    std::memcpy(pColorBuffer->surface.mipOffset, pTexture->surface.mipOffset, sizeof(u32) * 13);
    pColorBuffer->surface.imagePtr = pTexture->surface.imagePtr;
    pColorBuffer->surface.mipPtr = pTexture->surface.mipPtr;
    pColorBuffer->viewMip = mipLevel;

    GX2InitColorBufferRegs(pColorBuffer);
}

}
