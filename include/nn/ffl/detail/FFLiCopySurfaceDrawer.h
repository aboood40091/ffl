#ifndef FFLI_COPY_SURFACE_DRAWER_H_
#define FFLI_COPY_SURFACE_DRAWER_H_

#include <nn/ffl/types.h>

class FFLiCopySurfaceDrawer
{
public:
    FFLiCopySurfaceDrawer();
    ~FFLiCopySurfaceDrawer();

private:
    f32*    m_PosBuf;   // FFLVec2*
    f32*    m_TexBuf;   // FFLVec2*
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceDrawer) == 8);

#endif // FFLI_COPY_SURFACE_DRAWER_H_
