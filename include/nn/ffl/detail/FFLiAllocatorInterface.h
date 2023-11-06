#ifndef FFLI_ALLOCATOR_INTERFACE_H_
#define FFLI_ALLOCATOR_INTERFACE_H_

#include <nn/ffl/types.h>

class FFLiAllocatorInterface
{
public:
    virtual ~FFLiAllocatorInterface()
    {
    }

    virtual void* Allocate(u32 size) = 0;
    virtual void Free(void* ptr) = 0;
};
NN_STATIC_ASSERT(sizeof(FFLiAllocatorInterface) == 4);

#endif // FFLI_ALLOCATOR_INTERFACE_H_
