#ifndef FFLI_ZLIB_ALLOCATOR_H_
#define FFLI_ZLIB_ALLOCATOR_H_

#include <nn/ffl/types.h>

class FFLiAllocatorInterface;

class FFLiZlibAllocator
{
public:
    FFLiZlibAllocator(FFLiAllocatorInterface& interface);

    static void* Allocate(void* opaque, u32 items, u32 size);
    static void Free(void* opaque, void* address);

private:
    FFLiAllocatorInterface* m_pInterface;
};
NN_STATIC_ASSERT(sizeof(FFLiZlibAllocator) == 4);

#endif // FFLI_ZLIB_ALLOCATOR_H_
