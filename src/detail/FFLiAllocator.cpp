#include <nn/ffl/detail/FFLiAllocator.h>

#include <misc/rio_MemUtil.h>

FFLiAllocator::FFLiAllocator()
{
}

FFLiAllocator::~FFLiAllocator()
{
}

void FFLiAllocator::Init(void* pBuffer, u32 size)
{
    // TODO: Custom heap implementation using "pBuffer" and "size"
}

bool FFLiAllocator::IsValid() const
{
    return true;
}

void* FFLiAllocator::Allocate(u32 size, u32 alignment)
{
    return rio::MemUtil::alloc(size, alignment);
}

void FFLiAllocator::Free(void* ptr)
{
    rio::MemUtil::free(ptr);
}
