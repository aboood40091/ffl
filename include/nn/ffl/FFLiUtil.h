#ifndef FFLI_UTIL_H_
#define FFLI_UTIL_H_

#include <nn/ffl/types.h>

template <typename T>
T FFLiMax(T a, T b)
{
    if (a <= b) return b;
    else        return a;
}

void FFLiCopyWcharT2U16(u16* dst, u32 size, const wchar_t* src);

#endif // FFLI_UTIL_H_
