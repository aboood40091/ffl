#include <nn/ffl/detail/FFLiAllocator.h>

FFLiAllocator::FFLiAllocator()
    : m_HeapHandle(MEM_HEAP_INVALID_HANDLE)
{
}

FFLiAllocator::~FFLiAllocator()
{
    if (IsValid())
        MEMDestroyExpHeap(m_HeapHandle);
}

void FFLiAllocator::Init(void* pBuffer, u32 size)
{
    m_HeapHandle = MEMCreateExpHeap(pBuffer, size);
}

bool FFLiAllocator::IsValid() const
{
    return m_HeapHandle != MEM_HEAP_INVALID_HANDLE;
}

void* FFLiAllocator::Allocate(u32 size, u32 alignment)
{
    return MEMAllocFromExpHeapEx(m_HeapHandle, size, alignment);
}

void FFLiAllocator::Free(void* ptr)
{
    MEMFreeToExpHeap(m_HeapHandle, ptr);
}