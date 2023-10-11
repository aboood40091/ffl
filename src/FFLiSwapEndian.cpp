#include <nn/ffl/FFLiSwapEndian.h>

namespace {

u32 TypeToSize(FFLiSwapEndianType type)
{
    switch (type)
    {
    case FFLI_SWAP_ENDIAN_TYPE_U8:
        return 1;
    case FFLI_SWAP_ENDIAN_TYPE_U16:
        return 2;
    case FFLI_SWAP_ENDIAN_TYPE_U32:
        return 4;
    }
    return 0;
}

}

u32 FFLiSwapEndianGroup(void* ptr, const FFLiSwapEndianDesc* pDesc, u32 num)
{
    void* pPtr = ptr;

    for (u32 i = 0; i < num; i++)
    {
        const FFLiSwapEndianDesc& desc = pDesc[i];

        switch (desc.type)
        {
        case FFLI_SWAP_ENDIAN_TYPE_U8:
            FFLiSwapEndianArrayImpl<u8>(static_cast<u8*>(pPtr), desc.size);
            break;
        case FFLI_SWAP_ENDIAN_TYPE_U16:
            FFLiSwapEndianArrayImpl<u16>(static_cast<u16*>(pPtr), desc.size);
            break;
        case FFLI_SWAP_ENDIAN_TYPE_U32:
            FFLiSwapEndianArrayImpl<u32>(static_cast<u32*>(pPtr), desc.size);
            break;
        }

        pPtr = static_cast<u8*>(pPtr) + TypeToSize(desc.type) * desc.size;
    }

    return static_cast<u8*>(pPtr) - static_cast<u8*>(ptr);
}
