#ifndef FFLI_ALLOCATOR_H_
#define FFLI_ALLOCATOR_H_

#include <nn/ffl/types.h>

#include <cafe/mem.h>

class FFLiAllocator
{
public:
    FFLiAllocator();
    ~FFLiAllocator();

    void Init(void* pBuffer, u32 size);

    bool IsValid() const;

    void* Allocate(u32 size, u32 alignment = 4);
    void Free(void* ptr);

private:
    MEMHeapHandle   m_HeapHandle;
};
NN_STATIC_ASSERT(sizeof(FFLiAllocator) == 4);

#endif // FFLI_ALLOCATOR_H_
