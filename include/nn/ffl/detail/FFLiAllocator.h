#ifndef FFLI_ALLOCATOR_H_
#define FFLI_ALLOCATOR_H_

#include <nn/ffl/types.h>

#include <cafe/mem.h>

class FFLiAllocator
{
public:
    FFLiAllocator();
    ~FFLiAllocator();

private:
    MEMHeapHandle   m_HeapHandle;
};
NN_STATIC_ASSERT(sizeof(FFLiAllocator) == 4);

#endif // FFLI_ALLOCATOR_H_
