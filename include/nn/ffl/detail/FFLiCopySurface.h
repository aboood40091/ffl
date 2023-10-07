#ifndef FFLI_COPY_SURFACE_H_
#define FFLI_COPY_SURFACE_H_

#include <nn/ffl/detail/FFLiCopySurfaceDrawer.h>
#include <nn/ffl/detail/FFLiCopySurfaceShader.h>

class FFLiBufferAllocator;

class FFLiCopySurface
{
public:
    FFLiCopySurface(FFLiBufferAllocator* pAllocator);
    ~FFLiCopySurface();

    static u32 GetBufferSize();

    void SetupGPU();

    void Begin();
    void Execute(GX2Surface* pDstSurface, u32 dstMipLevel, const GX2Surface* pSrcSurface, u32 srcMipLevel);

    bool CanInitCharModel(bool isSetupGPU, bool compressTexture) const;

private:
    FFLiCopySurfaceShader   m_Shader;
    FFLiCopySurfaceDrawer   m_Drawer;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurface) == 0x44);

#endif // FFLI_COPY_SURFACE_H_
