#ifndef FFLI_ZLIB_ALLOCATOR_H_
#define FFLI_ZLIB_ALLOCATOR_H_

#include <nn/ffl/types.h>

class FFLiAllocatorInterface;

class FFLiZlibAllocator
{
private:
    FFLiAllocatorInterface* m_pInterface;
};
NN_STATIC_ASSERT(sizeof(FFLiZlibAllocator) == 4);

#endif // FFLI_ZLIB_ALLOCATOR_H_
