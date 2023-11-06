#include <nn/ffl/detail/FFLiBug.h>

#include <gpu/rio_Drawer.h>

void FFLiBugEndianSwap(void* ptr, u32 size)
{
}

u32 FFLiBugCanSwapSize(u32 size)
{
    return (size + (4 - 1)) & ~(4 - 1);
}

void* FFLiBugVgtFixedIndexPtr(void* ptr)
{
    return (u8*)ptr + (rio::Drawer::cIdxAlignment - 4);
}

void* FFLiBugVgtFixedIndexOriginalPtr(void* ptr)
{
    return (u8*)ptr - (rio::Drawer::cIdxAlignment - 4);
}

u32 FFLiBugCanVgtFixedIndexSize(u32 size)
{
    return size + (rio::Drawer::cIdxAlignment - 4);
}
