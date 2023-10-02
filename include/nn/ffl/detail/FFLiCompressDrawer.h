#ifndef FFLI_COMPRESS_DRAWER_H_
#define FFLI_COMPRESS_DRAWER_H_

#include <nn/ffl/FFLVec.h>

class FFLiBufferAllocator;

class FFLiCompressDrawer
{
public:
    FFLiCompressDrawer();
    ~FFLiCompressDrawer();  // Deleted in NSMBU

    static u32 GetBufferSize();

    void SetupCPU(FFLiBufferAllocator* pAllocator);
    void SetupGPU();

    void SetAttributeBuffer(u32 positionBufferIndex, u32 texCoordBufferIndex);
    void Draw();

private:
    FFLVec3*    m_pPositionBuffer;
    FFLVec2*    m_pTexCoordBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressDrawer) == 8);

#endif // FFLI_COMPRESS_DRAWER_H_
