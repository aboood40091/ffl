#ifndef FFLI_BUFFER_ALLOCATOR_H_
#define FFLI_BUFFER_ALLOCATOR_H_

#include <nn/ffl/types.h>

class FFLiBufferAllocator
{
public:
    void Init(void* pBuffer, u32 size);

    void* Allocate(u32 size, u32 alignment);
    void* Allocate(u32 size);

    u32 GetRestSize() const;

    bool CanAllocate(u32 size, u32 alignment) const;

private:
    void*   m_pBuffer;
    u32     m_Size;
};
NN_STATIC_ASSERT(sizeof(FFLiBufferAllocator) == 8);

#endif // FFLI_BUFFER_ALLOCATOR_H_
