#ifndef FFLI_COPY_SURFACE_H_
#define FFLI_COPY_SURFACE_H_

#include <nn/ffl/detail/FFLiCopySurfaceDrawer.h>
#include <nn/ffl/detail/FFLiCopySurfaceShader.h>

#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>

class FFLiCopySurface
{
public:
    FFLiCopySurface();
    ~FFLiCopySurface();

    void SetupGPU();

    void Begin();
    void End();

    void Execute(agl::TextureData* pTextureData, u32 dstMipLevel, u32 srcMipLevel);

    bool CanInitCharModel(bool isSetupGPU) const;

private:
    FFLiCopySurfaceShader   m_Shader;
    FFLiCopySurfaceDrawer   m_Drawer;
    agl::RenderBuffer       m_RenderBuffer;
    agl::RenderTargetColor  m_ColorTarget;
};
//NN_STATIC_ASSERT(sizeof(FFLiCopySurface) == 0x44);

#endif // FFLI_COPY_SURFACE_H_
