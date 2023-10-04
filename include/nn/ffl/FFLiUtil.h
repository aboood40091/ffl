#ifndef FFLI_UTIL_H_
#define FFLI_UTIL_H_

#include <nn/ffl/types.h>

#include <algorithm>

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

template <typename T>
T FFLiSwapEndianImpl(T value)
{
    const u32 count = sizeof(T);
    NN_STATIC_ASSERT(count % 2 == 0);
    const u32 count_2 = count / 2;

    union {
        T   value;
        u8  data[count];
    } value_ = { .value = value };

    for (u32 i = 0; i < count_2; i++)
        std::swap(value_.data[i], value_.data[count - 1 - i]);

    return value_.value;
}

template <typename T>
void FFLiSwapEndianArrayImpl(T* pArray, u32 size)
{
    for (u32 i = 0; i < size; i++)
        pArray[i] = FFLiSwapEndianImpl<T>(pArray[i]);
}

void FFLiCopyWcharT2U16(u16* dst, u32 size, const wchar_t* src);

#endif // FFLI_UTIL_H_
