#ifndef FFLI_SWAP_ENDIAN_H_
#define FFLI_SWAP_ENDIAN_H_

#include <nn/ffl/types.h>

#include <cstddef>

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

void FFLiSwapEndianGroup(void* ptr, const FFLiSwapEndianDesc* pDesc, u32 num);

template <typename T>
T FFLiSwapEndianImpl(T value)
{
    constexpr std::size_t count = sizeof(T);
    NN_STATIC_ASSERT(count == 1 || count == 2 || count == 4 || count == 8);

    if constexpr (count == 2)
        return __builtin_bswap16(value);
    else if constexpr (count == 4)
        return __builtin_bswap32(value);
    else if constexpr (count == 8)
        return __builtin_bswap64(value);
    else
        return value;
}

template <typename T>
void FFLiSwapEndianArrayImpl(T* pArray, u32 size)
{
    for (u32 i = 0; i < size; i++)
        pArray[i] = FFLiSwapEndianImpl<T>(pArray[i]);
}

#endif // FFLI_SWAP_ENDIAN_H_
