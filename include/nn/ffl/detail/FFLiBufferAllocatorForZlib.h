#ifndef FFLI_BUFFER_ALLOCATOR_FOR_ZLIB_H_
#define FFLI_BUFFER_ALLOCATOR_FOR_ZLIB_H_

#include <nn/ffl/detail/FFLiAllocatorInterface.h>
#include <nn/ffl/detail/FFLiBufferAllocator.h>

class FFLiBufferAllocatorForZlib : public FFLiAllocatorInterface
{
public:
    FFLiBufferAllocatorForZlib(void* pBuffer, u32 size);
    virtual ~FFLiBufferAllocatorForZlib();

    virtual void* Allocate(u32 size);
    virtual void* Allocate(u32 size, u32 alignment);
    virtual void Free(void* ptr);
private:
    FFLiBufferAllocator m_BufferAllocator;
};
NN_STATIC_ASSERT(sizeof(FFLiBufferAllocatorForZlib) == 0xC);

#endif // FFLI_BUFFER_ALLOCATOR_FOR_ZLIB_H_
