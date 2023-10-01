#ifndef FFLI_COMPRESS_DRAWER_H_
#define FFLI_COMPRESS_DRAWER_H_

#include <nn/ffl/FFLVec.h>

class FFLiCompressDrawer
{
public:
    FFLiCompressDrawer();
    ~FFLiCompressDrawer();

private:
    FFLVec3*    m_pPositionBuffer;
    FFLVec2*    m_pTexCoordBuffer;
};
NN_STATIC_ASSERT(sizeof(FFLiCompressDrawer) == 8);

#endif // FFLI_COMPRESS_DRAWER_H_
