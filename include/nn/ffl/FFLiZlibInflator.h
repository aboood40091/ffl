#ifndef FFLI_ZLIB_INFLATOR_H_
#define FFLI_ZLIB_INFLATOR_H_

#include <nn/ffl/FFLiZlibInterface.h>

#include <nn/ffl/detail/FFLiZlibAllocator.h>

#include <zlib.h>

class FFLiZlibInflator : public FFLiZlibInterface
{
public:
    FFLiZlibInflator(FFLiAllocatorInterface& allocatorInterface, s32 windowBits);
    virtual ~FFLiZlibInflator();

    virtual s32 Process(void** ppDst, u32* pDstSize, const void** ppSrc, u32* pSrcSize, s32 flush);
    virtual u32 TotalInputSize() const;
    virtual u32 TotalOutputSize() const;

private:
    FFLiZlibAllocator   m_ZlibAllocator;
    z_stream            m_Stream;
    bool                m_IsStreamEnd;
};
NN_STATIC_ASSERT(sizeof(FFLiZlibInflator) == 0x44);

#endif // FFLI_ZLIB_INFLATOR_H_