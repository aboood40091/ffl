#include <nn/ffl/detail/FFLiAllocatorInterface.h>
#include <nn/ffl/detail/FFLiZlibAllocator.h>

FFLiZlibAllocator::FFLiZlibAllocator(FFLiAllocatorInterface& interface)
    : m_pInterface(&interface)
{
}

void* FFLiZlibAllocator::Allocate(void* opaque, u32 items, u32 size)
{
    return static_cast<FFLiZlibAllocator*>(opaque)->m_pInterface->Allocate(size * items);
}

void FFLiZlibAllocator::Free(void* opaque, void* address)
{
    static_cast<FFLiZlibAllocator*>(opaque)->m_pInterface->Free(address);
}
