#ifndef FFLI_COPY_SURFACE_H_
#define FFLI_COPY_SURFACE_H_

#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/detail/FFLiCopySurfaceDrawer.h>
#include <nn/ffl/detail/FFLiCopySurfaceShader.h>

class FFLiCopySurface
{
public:
    FFLiCopySurface();
    ~FFLiCopySurface();

    void SetupGPU();

    void Begin();
    void End();

    void Execute(GX2Surface* pDstSurface, u32 dstMipLevel, const GX2Surface* pSrcSurface, u32 srcMipLevel);

private:
    void SetupSrcSurface(const GX2Surface* pSurface, u32 mipLevel);
    void SetupDstSurface(GX2Surface* pSurface, u32 mipLevel);

private:
    FFLiCopySurfaceShader   m_Shader;
    FFLiCopySurfaceDrawer   m_Drawer;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurface) == 0x44);

#endif // RIO_IS_CAFE

#endif // FFLI_COPY_SURFACE_H_
