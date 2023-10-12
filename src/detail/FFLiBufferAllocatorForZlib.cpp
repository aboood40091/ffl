#include <nn/ffl/detail/FFLiBufferAllocatorForZlib.h>

FFLiBufferAllocatorForZlib::FFLiBufferAllocatorForZlib(void* pBuffer, u32 size)
{
    m_BufferAllocator.Init(pBuffer, size);
}

FFLiBufferAllocatorForZlib::~FFLiBufferAllocatorForZlib()
{
}

void* FFLiBufferAllocatorForZlib::Allocate(u32 size)
{
    return m_BufferAllocator.Allocate(size);
}

void FFLiBufferAllocatorForZlib::Free(void* ptr)
{
}
