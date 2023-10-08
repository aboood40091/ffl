#ifndef FFLI_SWAP_ENDIAN_H_
#define FFLI_SWAP_ENDIAN_H_

#include <nn/ffl/types.h>

#include <algorithm>

enum FFLiSwapEndianType
{
    FFLI_SWAP_ENDIAN_TYPE_U8    = 0,
    FFLI_SWAP_ENDIAN_TYPE_U16   = 1,
    FFLI_SWAP_ENDIAN_TYPE_U32   = 2
};

struct FFLiSwapEndianDesc
{
    FFLiSwapEndianType  type;
    u32                 size;
};
NN_STATIC_ASSERT(sizeof(FFLiSwapEndianDesc) == 8);

u32 FFLiSwapEndianGroup(void* ptr, const FFLiSwapEndianDesc* pDesc, u32 num);

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

#endif // FFLI_SWAP_ENDIAN_H_
