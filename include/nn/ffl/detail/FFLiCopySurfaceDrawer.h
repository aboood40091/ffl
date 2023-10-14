#ifndef FFLI_COPY_SURFACE_DRAWER_H_
#define FFLI_COPY_SURFACE_DRAWER_H_

#include <nn/ffl/FFLVec.h>

class FFLiBufferAllocator;

class FFLiCopySurfaceDrawer
{
public:
    FFLiCopySurfaceDrawer();
    ~FFLiCopySurfaceDrawer();

    void SetupCPU(FFLiBufferAllocator* pAllocator);
    void SetupGPU();

    void SetAttributeBuffer(u32 positionBufferIndex, u32 texCoordBufferIndex);
    void Draw();

    static u32 GetBufferSize();

private:
    FFLVec2*    m_pPositionBuffer;
    FFLVec2*    m_pTexCoordBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiCopySurfaceDrawer) == 8);

#endif // FFLI_COPY_SURFACE_DRAWER_H_
