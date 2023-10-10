#ifndef FFLI_UTIL_H_
#define FFLI_UTIL_H_

#include <nn/ffl/types.h>

template <typename T>
T FFLiMin(T a, T b)
{
    if (a >= b) return b;
    else        return a;
}

template <typename T>
T FFLiMax(T a, T b)
{
    if (a <= b) return b;
    else        return a;
}

u32 FFLiRoundUp(u32 value, u32 multiple);
void* FFLiRoundUpPtr(void* ptr, u32 multiple);

void FFLiCopyWcharT2U16(u16* dst, u32 size, const wchar_t* src);
bool FFLiIsNumberCharCode(u16 c);

#endif // FFLI_UTIL_H_
