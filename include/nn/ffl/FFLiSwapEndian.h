#ifndef FFLI_SWAP_ENDIAN_H_
#define FFLI_SWAP_ENDIAN_H_

#include <nn/ffl/types.h>

#include <numeric>

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

template <typename T, std::size_t N>
struct SwapEndianHelper
{
    static T swap(T value) = delete;
};

template <typename T>
struct SwapEndianHelper<T, 1>
{
    static T swap(T value)
    {
        return value;
    }
};

template <typename T>
struct SwapEndianHelper<T, 2>
{
    static T swap(T value)
    {
        u16 tmp = std::bit_cast<u16>(value);
        tmp = __builtin_bswap16(tmp);
        value = std::bit_cast<T>(tmp);
        return value;
    }
};

template <typename T>
struct SwapEndianHelper<T, 4>
{
    static T swap(T value)
    {
        u32 tmp = std::bit_cast<u32>(value);
        tmp = __builtin_bswap32(tmp);
        value = std::bit_cast<T>(tmp);
        return value;
    }
};

template <typename T>
T FFLiSwapEndianImpl(T value)
{
    constexpr std::size_t count = sizeof(T);
    NN_STATIC_ASSERT(count == 1 || count == 2 || count == 4);
    return SwapEndianHelper<T, count>::swap(value);
}

template <typename T>
void FFLiSwapEndianArrayImpl(T* pArray, u32 size)
{
    for (u32 i = 0; i < size; i++)
        pArray[i] = FFLiSwapEndianImpl<T>(pArray[i]);
}

#endif // FFLI_SWAP_ENDIAN_H_
