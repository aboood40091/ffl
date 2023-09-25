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

private:
    FFLiCopySurfaceShader   m_Shader;
    FFLiCopySurfaceDrawer   m_Drawer;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurface) == 0x44);

#endif // FFLI_COPY_SURFACE_H_
