#ifndef FFLI_COPY_SURFACE_DRAWER_H_
#define FFLI_COPY_SURFACE_DRAWER_H_

#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <nn/ffl/FFLVec.h>

class FFLiCopySurfaceDrawer
{
public:
    FFLiCopySurfaceDrawer();
    ~FFLiCopySurfaceDrawer();

    void SetupCPU();
    void SetupGPU();

    void SetAttributeBuffer(u32 positionBufferIndex, u32 texCoordBufferIndex);
    void Draw();

private:
    FFLVec2*    m_pPositionBuffer;
    FFLVec2*    m_pTexCoordBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceDrawer) == 8);

#endif // RIO_IS_CAFE

#endif // FFLI_COPY_SURFACE_DRAWER_H_
