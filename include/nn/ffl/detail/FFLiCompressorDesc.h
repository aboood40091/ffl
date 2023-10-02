#ifndef FFLI_COMPRESSOR_DESC_H_
#define FFLI_COMPRESSOR_DESC_H_

#include <nn/ffl/detail/FFLiCompressUtil.h>

#include <cafe/gx2.h>

struct FFLiCompressorDescBC1
{
    static const GX2SurfaceFormat FORMAT = GX2_SURFACE_FORMAT_T_BC1_UNORM;

    static void SetupColorBuffer(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel)
    {
        FFLiSetupColorBufferForBC1(pColorBuffer, pTexture, mipLevel);
    }
};

struct FFLiCompressorDescBC3
{
    static const GX2SurfaceFormat FORMAT = GX2_SURFACE_FORMAT_T_BC3_UNORM;

    static void SetupColorBuffer(GX2ColorBuffer* pColorBuffer, GX2Texture* pTexture, u32 mipLevel)
    {
        FFLiSetupColorBufferForBC3(pColorBuffer, pTexture, mipLevel);
    }
};

#endif // FFLI_COMPRESSOR_DESC_H_
