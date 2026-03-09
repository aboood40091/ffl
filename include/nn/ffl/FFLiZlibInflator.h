#ifndef FFLI_ZLIB_INFLATOR_H_
#define FFLI_ZLIB_INFLATOR_H_

#include <nn/ffl/FFLiZlibInterface.h>

#include <zlib.h>

class FFLiZlibInflator : public FFLiZlibInterface
{
public:
    FFLiZlibInflator(s32 windowBits);
    virtual ~FFLiZlibInflator();

    virtual s32 Process(void** ppDst, u32* pDstSize, const void** ppSrc, u32* pSrcSize, s32 flush);

private:
    u32                 _0[4 / sizeof(u32)];    // Deleted
    z_stream            m_Stream;
    bool                m_IsStreamEnd;
};
NN_STATIC_ASSERT(sizeof(FFLiZlibInflator) == NN_ALIGN_FOR_PTR(NN_ALIGN_TO(sizeof(FFLiZlibInterface) + 4, alignof(z_stream)) + sizeof(z_stream) + 1));

#endif // FFLI_ZLIB_INFLATOR_H_
