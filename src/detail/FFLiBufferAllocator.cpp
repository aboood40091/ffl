#include <nn/ffl/FFLiUtil.h>

#include <nn/ffl/detail/FFLiBufferAllocator.h>

void* FFLiAllocateBufferAllocator(FFLiBufferAllocator* pAllocator, u32 size, u32 alignment)
{
    return pAllocator->Allocate(size, alignment);
}

void* FFLiAllocateBufferAllocator(FFLiBufferAllocator* pAllocator, u32 size)
{
    return pAllocator->Allocate(size);
}

void FFLiBufferAllocator::Init(void* pBuffer, u32 size)
{
    m_pBuffer = pBuffer;
    m_Size = size;
}

void* FFLiBufferAllocator::Allocate(u32 size, u32 alignment)
{
    u32 bufferPtr = (u32)m_pBuffer;
    void* ptr = (void*)((bufferPtr + (alignment - 1)) & ~(alignment - 1));
    m_pBuffer = (void*)((u32)ptr + size);
    m_Size -= (u32)ptr - bufferPtr + size;
    return ptr;
}

void* FFLiBufferAllocator::Allocate(u32 size)
{
    return Allocate(size, 1);
}

u32 FFLiBufferAllocator::GetRestSize() const
{
    return m_Size;
}

bool FFLiBufferAllocator::CanAllocate(u32 size, u32 alignment) const
{
    u32 bufferPtr = (u32)m_pBuffer;
    u32 alignedSize = (FFLiRoundUp(bufferPtr, alignment) + size) - bufferPtr;
    return alignedSize <= GetRestSize();
}
