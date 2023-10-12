#include <nn/ffl/detail/FFLiBug.h>

#include <cafe/gx2.h>

void FFLiBugEndianSwap(void* ptr, u32 size)
{
}

u32 FFLiBugCanSwapSize(u32 size)
{
    return size + (4 - 1) & ~(4 - 1);
}

void* FFLiBugVgtFixedIndexPtr(void* ptr)
{
    return (u8*)ptr + (GX2_INDEX_BUFFER_ALIGNMENT - 4);
}

u32 FFLiBugCanVgtFixedIndexSize(u32 size)
{
    return size + (GX2_INDEX_BUFFER_ALIGNMENT - 4);
}
