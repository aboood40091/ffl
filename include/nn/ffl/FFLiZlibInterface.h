#ifndef FFLI_ZLIB_INTERFACE_H_
#define FFLI_ZLIB_INTERFACE_H_

#include <nn/ffl/types.h>

class FFLiZlibInterface
{
public:
    virtual ~FFLiZlibInterface()
    {
    }

    virtual s32 Process(void** ppDst, u32* pDstSize, const void** ppSrc, u32* pSrcSize, s32 flush) = 0;
};
NN_STATIC_ASSERT(sizeof(FFLiZlibInterface) == 4);

#endif // FFLI_ZLIB_INTERFACE_H_
